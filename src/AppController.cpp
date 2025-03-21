#include "AppController.h"

#include <iostream>

#include "Action.h"
#include "exception/NotFoundException.h"

using namespace qrz;

AppController::AppController(Configuration *config, TableModel *table) : config(config), tableModel(table)
{
}

/**
 * @brief Initializes the application by setting up the necessary configurations and checking for authentication.
 *
 * This function is responsible for initializing the application. It checks if a callsign is set in the configuration,
 * and if not, prompts the user to enter their callsign. It also sets the callsign in the QRZ API client. Then, it checks
 * if a session key and session expiration are set in the configuration. If not, it refreshes the token by fetching a new
 * session key and expiration from the QRZ API. If the token is already expired, it refreshes it as well. Finally, it sets
 * the session key and expiration in the QRZ API client.
 */
void AppController::initialize()
{
	client.setUsername(config->getUsername());
	client.setSessionKey(config->getSessionKey());
	client.setSessionExpiration(config->getSessionExpiration());
}

bool AppController::preflight()
{
	std::string userCall = config->getUsername();
	std::string password = config->getPassword();

	client.setUsername(userCall);
	client.setSessionKey(config->getSessionKey());
	client.setSessionExpiration(config->getSessionExpiration());

	if (!config->hasSessionKey() || !config->hasSessionExpiration() || !client.tokenIsValid())
	{
		refreshToken();
	}
	else if (userCall.empty() || password.empty())
	{
		emit credentialsNeeded();
		return false;
	}

	return true;
}

/**
 * @brief Handles a command by performing an action based on the command's action type.
 *
 * This function takes an AppCommand object and performs different operations based on the command's action type.
 * The action type determines the type of operation to be performed.
 *
 * @param command The AppCommand object representing the command to be executed.
 */
bool AppController::handleCommand(const AppCommand &command)
{
	if(!preflight())
	{
		return false;
	}

	switch (command.getAction())
	{
		case Action::CALLSIGN_ACTION:
			return fetchAndRenderCallsigns(command.getSearchTerms());
			break;
		case Action::BIO_ACTION:
			return fetchAndRenderBios(command.getSearchTerms());
			break;
		case Action::DXCC_ACTION:
			return fetchAndRenderDXCC(command.getSearchTerms());
			break;
	}

	return false;
}

void AppController::fetchCallsign(const std::string &call, QrzCallsignResponseCallback callback)
{
	std::vector<Callsign> callsigns = fetchCallsignRecords({call});

	if (callsigns.size() > 0)
	{
		callback(callsigns.at(0));
	}
}

std::string AppController::fetchBio(const std::string &call)
{
	std::vector<std::string> bios = fetchBios({call});
	return bios.at(0);
}

/**
 * @brief Fetches and renders callsigns based on the given search terms and output format.
 *
 * This function takes a set of search terms and an output format and fetches the callsign records using the fetchCallsignRecords function.
 * It then creates a renderer object based on the output format using the RendererFactory and renders the callsigns using the Render function.
 * After rendering, it updates the application configuration from the client state.
 *
 * @param searchTerms The set of search terms used to fetch the callsign records.
 * @param format The output format in which the callsigns should be rendered.
 */
bool AppController::fetchAndRenderCallsigns(const std::set<std::string> &searchTerms)
{
	bool status = true;

	try
	{
		const std::vector<Callsign> callsigns = fetchCallsignRecords(searchTerms);

		status = (callsigns.size() > 0);

		tableModel->addCallsigns(callsigns);

		updateConfigFromClientState();
	}
	catch(std::exception e)
	{
		status = false;
	}

	return status;
}

/**
* @brief Fetches and renders DXCC records based on the given search terms and output format.
*
* This function takes a set of search terms and an output format and fetches the DXCC records using the fetchDXCCRecords function.
* It then creates a renderer object based on the output format using the RendererFactory and renders the DXCC records using the Render function.
* After rendering, it updates the application configuration from the client state.
*
* @param searchTerms The set of search terms used to fetch the DXCC records.
* @param format The output format in which the DXCC records should be rendered.
*/
bool AppController::fetchAndRenderDXCC(const std::set<std::string> &searchTerms)
{
	try
	{
		const std::vector<DXCC> dxccRecords = fetchDXCCRecords(searchTerms);

		// TODO do something with the records

		updateConfigFromClientState();
	}
	catch(std::exception e)
	{
		return false;
	}

	return true;
}

/**
 * @brief Fetches and renders bios based on the given search terms.
 *
 * This function fetches the specified callsign records and renders them.
 *
 * Unlike callsigns and DXCC records, bio HTML content is rendered directly as it is received from the QRZ API.
 *
 * @param searchTerms The set of search terms used to fetch the bio content.
 */
bool AppController::fetchAndRenderBios(const std::set<std::string> &searchTerms)
{
	try
	{
		std::vector<std::string> bios = fetchBios(searchTerms);

		//TODO do something with bios

		updateConfigFromClientState();
	}
	catch(std::exception e)
	{
		return false;
	}

	return true;
}

/**
 * @brief Fetches the callsign records based on the given search terms.
 *
 * This function fetches the callsign records based on the provided search terms.
 * It makes API calls to retrieve the callsigns and stores them in a vector.
 * It handles authentication errors and retries the API call after refreshing the token.
 * It also displays a progress bar to show the progress of fetching the callsigns.
 *
 * @param searchTerms The set of search terms used to fetch the callsign records.
 * @return A vector of Callsign objects representing the fetched callsign records.
 *
 * @note This function assumes that the necessary APIs and client objects are properly initialized before calling this function.
 * @note This function prints any errors encountered during the API calls to the standard error stream.
 * @note This function sets the progress bar option and progress values to reflect the progress of fetching the callsigns.
 */
std::vector<Callsign> AppController::fetchCallsignRecords(const std::set<std::string> &searchTerms)
{
	// Buffer for the output
	std::vector<Callsign> callsigns;

	// Error buffer, we will display the errors after all API calls have been made and progress bar is removed
	std::vector<std::string> errors;

	// Display our progress bar
	//auto bar = buildProgressBar();

	// Progress bar value
	auto progress = 0.0f;

	// Amount to advance the progress bar after each call
	float tickSize = 100/searchTerms.size();

	// Use an iterator so we cen reset the current element in the event of authentication error
	auto it = searchTerms.begin();
	while (it != searchTerms.end())
	{
		const std::string &call = *it;

		if(invalidCallsigns.contains(call))
		{
			it++;
			continue;
		}

		try
		{
			// Hide the cursor - we do this on every iteration in case we hit an authentication error previously
			//showConsoleCursor(false);

			// Update progress bar
			//bar->setOption(indicators::option::PostfixText{std::format("Fetching {:s}", call)});
			//bar->setProgress(progress);

			progress += tickSize;

			// Fetch the callsign and add it to the output buffer
			Callsign callsign = client.fetchCallsign(call);

			callsigns.push_back(callsign);

			// Reset the fail counter
			resetFailedCallCount();

			it++;
		}
		catch (AuthenticationException &e)
		{
			// If we have not exceeded the max fail count, try to authenticate
			if (m_failedCallCount < m_maxFailedCallCount)
			{
				// Hide the progress bar and give the cursor back
				//eraseLine();
				//showConsoleCursor(true);

				// Ask the user for their password, and refresh the bearer token
				refreshToken();

				// Increment the error counter so we don't do this forever
				m_failedCallCount++;
			}
			else
			{
				// Save the error for display after we have mde all of our API calls
				errors.push_back(std::format("QRZ API Error: {:s}", e.what()));
				it++;
			}
		}
		catch (NotFoundException &e)
		{
			if(!invalidCallsigns.contains(call))
			{
				errors.emplace_back(e.what());
				invalidCallsigns.insert(call);
			}

			it++;
		}
		catch (std::exception &e)
		{
			// Save the error for display after we have mde all of our API calls
			errors.emplace_back(e.what());

			it++;
		}
	}

	// Display the errors, if any
	if(!errors.empty())
	{
		std::ostringstream msg;
		for(const std::string& error : errors)
		{
			msg << error << "\n\n";
		}

		emit displayError(msg.str());
	}

	return callsigns;
}

/**
 * @brief Fetches DXCC records based on the given search terms.
 *
 * This function fetches DXCC records based on the provided search terms. It uses the QRZ API client to fetch the records
 * for each term in the searchTerms set. If an authentication error occurs, it retries the call after refreshing the
 * authentication token. Any errors that occur during the fetch process are stored in the errors vector.
 *
 * @param searchTerms The set of search terms used to fetch the DXCC records.
 * @return A vector of DXCC objects representing the fetched DXCC records.
 */
std::vector<DXCC> AppController::fetchDXCCRecords(const std::set<std::string> &searchTerms)
{
	std::vector<DXCC> dxccs;
	std::vector<std::string> errors;

	showConsoleCursor(false);

	//auto bar = buildProgressBar();

	auto progress = 0.0f;

	float tickSize = 100/searchTerms.size();

	// Use an iterator so we cen reset the current element in the event of authentication error
	auto it = searchTerms.begin();
	while (it != searchTerms.end())
	{
		const std::string &term = *it;

		try
		{
			//bar->setOption(indicators::option::PostfixText{std::format("Fetching {:s}", term)});
			//bar->setProgress(progress);

			progress += tickSize;

			dxccs.push_back(client.fetchDXCC(term));

			resetFailedCallCount();

			it++;
		}
		catch (AuthenticationException &e)
		{
			if (m_failedCallCount < m_maxFailedCallCount)
			{
				refreshToken();

				m_failedCallCount++;
			}
			else
			{
				errors.push_back(std::format("QRZ API Error: {:s}", e.what()));
				it++;
			}
		}
		catch (std::exception &e)
		{
			errors.emplace_back(e.what());
			it++;
		}
	}

	//bar->setOption(indicators::option::PostfixText{""});
	//bar->setProgress(100);
	//eraseLine();
	//showConsoleCursor(true);

	for(const std::string& error : errors)
	{
		std::cerr << error << std::endl;
	}

	return dxccs;
}

/**
 * @brief Fetches and returns a vector of bios based on the given search terms.
 *
 * This function fetches the specified bio records and returns a vector of bio HTML strings.
 * It uses the QRZ API client to fetch the bios for each search term.
 *
 * @param searchTerms The set of search terms used to fetch the bios.
 * @return A vector of strings representing the fetched bios.
 *
 * @note This function assumes that the necessary APIs and client objects are properly initialized before calling this function.
 */
std::vector<std::string> AppController::fetchBios(const std::set<std::string> &searchTerms)
{
	std::vector<std::string> bios;
	std::vector<std::string> errors;

	// Use an iterator so we cen reset the current element in the event of authentication error
	auto it = searchTerms.begin();
	while (it != searchTerms.end())
	{
		const std::string &call = *it;

		try
		{
			bios.push_back(client.fetchBio(call));

			resetFailedCallCount();

			it++;
		}
		catch (AuthenticationException &e)
		{
			if (m_failedCallCount < m_maxFailedCallCount)
			{
				refreshToken();

				// Roll the iterator back to retry this call
				it = std::prev(it);

				m_failedCallCount++;
			}
			else
			{
				errors.push_back(std::format("QRZ API Error: {:s}", e.what()));
				it++;
			}
		}
		catch (std::exception &e)
		{
			errors.emplace_back(e.what());
			it++;
		}
	}

	for(const std::string& error : errors)
	{
		std::cerr << error << std::endl;
	}

	return bios;
}

/**
 * @brief Refreshes the access token by fetching a new token from the QRZ API
 *
 * This function refreshes the access token by fetching a new token from the QRZ API.
 * It retrieves the password from the configuration, and if it is empty, prompts the user to enter it.
 * The function then sets the username and password in the QRZ client, fetches a new token,
 * and updates the configuration with the new session information.
 *
 * @note This function assumes that the necessary APIs and client objects are properly initialized before calling this function.
 */
bool AppController::refreshToken()
{
	std::string userCall = config->getUsername();
	std::string password = config->getPassword();

	if(userCall.empty() || password.empty())
	{
		emit credentialsNeeded();
		return false;
	}

	client.setUsername(userCall);
	client.setPassword(password);

	client.fetchToken();

	updateConfigFromClientState();

	return true;
}

/**
* @brief Retrieves the user's callsign from the user.
*
* This function prompts the user to enter their callsign and reads the input from the console.
*
* @return A string representing the user's callsign.
*/
std::string AppController::getUserCallsignFromUser()
{
	std::string userCall;

	std::cout << "Enter your callsign:" << std::endl;

	std::cin >> userCall;

	return userCall;
}

/**
 * @brief Retrieves the password from the user.
 *
 * This function prompts the user to enter their password and reads the input from the console.
 *
 * @return A string representing the user's password.
 */
std::string AppController::getPasswordFromUser()
{
	std::string userCall = config->getUsername();

	std::cout << "Login required. Enter the QRZ password for " << userCall << ":" << std::endl;

	SetStdinEcho(false);

	std::string password;
	std::cin >> password;

	SetStdinEcho(true);

	// We only set the password here, so store
	config->setPassword(password);

	return password;
}

/**
 * @brief Updates the application configuration based on the state of the QRZ API client.
 *
 * This function updates the application configuration by setting the callsign, session key, and session expiration
 * from the QRZ API client. It then saves the configuration.
 *
 * @note This function assumes that the QRZ API client and application configuration objects are properly initialized.
 */
void AppController::updateConfigFromClientState()
{
	config->setUsername(client.getUsername());
	config->setSessionKey(client.getSessionKey());
	config->setSessionExpiration(client.getSessionExpiration());
}

/**
 * @brief Resets the counter for failed API calls.
 *
 * This function resets the counter for failed API calls. It sets the value of m_failedCallCount to 0.
 */
void AppController::resetFailedCallCount()
{
	m_failedCallCount = 0;
}

/**
 * @brief Changes the visibility of the console cursor.
 *
 * This function sets the visibility of the console cursor based on the value of the parameter `show`.
 * If `show` is `true`, the console cursor will be displayed. If `show` is `false`, the console cursor will be hidden.
 *
 * @param show A boolean value indicating whether to show or hide the console cursor.
 */
inline void AppController::showConsoleCursor(bool const show) {
	std::fputs(show ? "\033[?25h" : "\033[?25l", stderr);
}

#ifdef WIN32
/**
 * @brief Erases the current line in the console.
 *
 * This function erases the current line in the console, allowing for output to be overwritten.
 * It uses the escape sequences "\x1b[1A" and "\x1b[2K" to move the cursor to the beginning of the line and clear all characters.
 */
inline void AppController::eraseLine() {
	std::fputs("\x1b[1A", stderr);
	std::fputs("\x1b[2K", stderr);
}

/**
 * @brief Create a new instance of ProgressBar.
 *
 * This static member function creates a new instance of ProgressBar and returns it as a unique_ptr.
 * It creates an instance of the BlockProgressBar class, which is a concrete implementation of the ProgressBar interface.
 *
 * @return A unique_ptr to a ProgressBar object.
 */
std::unique_ptr<ProgressBar> AppController::buildProgressBar()
{
	return std::make_unique<DefaultProgressBar>();
}
#else
/**
* @brief Erases the current line in the console.
*
* This function erases the current line in the console, allowing for output to be overwritten.
* It uses the escape sequence "\r\033[K" to move the cursor to the beginning of the line and clear all characters.
*/
inline void AppController::eraseLine() {
	std::fputs("\r\033[K", stderr);
}

/**
 * @brief Create a new instance of ProgressBar.
 *
 * This static member function creates a new instance of ProgressBar and returns it as a unique_ptr.
 * It creates an instance of the BlockProgressBar class, which is a concrete implementation of the ProgressBar interface.
 *
 * @return A unique_ptr to a ProgressBar object.

std::unique_ptr<ProgressBar> AppController::buildProgressBar()
{
	return std::make_unique<BlockProgressBar>();
}
 */
#endif

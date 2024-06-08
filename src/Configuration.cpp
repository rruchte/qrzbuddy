#include "Configuration.h"

#include <iostream>

#include <Poco/Crypto/CipherFactory.h>
#include <Poco/Crypto/CipherKey.h>
#include <Poco/Crypto/CipherKeyImpl.h>

#include "Util.h"

using namespace qrz;

/**
 * @class Configuration
 *
 * The Configuration class represents a configuration file manager. It allows
 * reading and writing configuration values from a file. Configuration values
 * include username, password, session key, and session expiration.
 *
 * Example Usage:
 *
 * Configuration config;
 * config.setUsername("ABCD123");
 * config.setPassword("mypassword");
 * config.saveConfig();
 *
 * std::string username = config.getUsername();
 * std::string password = config.getPassword();
 *
 * Note: The configuration file is managed by QSettings
 */
Configuration::Configuration()
{
	settings = new QSettings();
}

Configuration::~Configuration()
{
	delete(settings);
}

/**
 * @brief Returns the username value from the configuration.
 *
 * This function retrieves the username value from the configuration.
 *
 * @return The username value as a string.
 */
std::string Configuration::getUsername()
{
	return getValue(f_username).toString().toStdString();
}

/**
* @brief Retrieves the password value from the configuration.
*
* This function retrieves the password value from the configuration file.
*
* @return The password value as a string.
*/
std::string Configuration::getPassword()
{
	if(hasPassword())
	{
		try
		{
			return decryptPassword(getValue(f_password).toString().toStdString());
		}
		catch (Poco::IOException &e)
		{
			std::cout << e.message() << std::endl;
		}
	}

	return "";
}

/**
 * @brief Retrieves the session key value from the configuration.
 *
 * This function retrieves the session key value from the configuration file.
 *
 * @return The session key value as a string.
 */
std::string Configuration::getSessionKey()
{
	return getValue(f_sessionKey).toString().toStdString();
}

/**
 * @brief Retrieves the session expiration value from the configuration file.
 *
 * This function retrieves the session expiration value from the configuration
 * file using the key "session_expiration". Session expiration represents the
 * duration of time in which a session remains valid.
 *
 * @return The session expiration value as a string.
 */
std::string Configuration::getSessionExpiration()
{
	return getValue(f_sessionExpiration).toString().toStdString();
}

/**
 * @brief Returns the value of the js8CallEnable configuration option.
 *
 * This function retrieves the value of the js8CallEnable configuration option from the configuration file.
 *
 * @return The value of the js8CallEnable configuration option as a boolean.
 */
bool Configuration::getJs8CallEnabled()
{
	return getValue(f_js8CallEnable).toBool();
}

/**
 * @brief Retrieves the value associated with the "js8call/host" key from the configuration.
 *
 * This function retrieves the value associated with the "js8call/host" key from the configuration file.
 *
 * @return The value associated with the "js8call/host" key as a string.
 */
std::string Configuration::getJs8CallHost()
{
	return getValue(f_js8CallHost).toString().toStdString();
}

/**
 * @brief Retrieves the JS8Call port value from the configuration.
 *
 * This function retrieves the JS8Call port value from the configuration file.
 *
 * @return The JS8Call port value as an integer.
 */
int Configuration::getJs8CallPort()
{
	return getValue(f_js8CallPort).toInt();
}

/**
 * @brief Retrieves the value associated with the "station/callsign" key from the configuration.
 *
 * This function retrieves the value associated with the "station/callsign" key from the configuration file.
 *
 * @return The value associated with the "station/callsign" key as a string.
 */
std::string Configuration::getCallsign()
{
	return getValue(f_callsign).toString().toStdString();
}

/**
 * @brief Retrieves the value associated with the "station/grid" key from the configuration.
 *
 * This function retrieves the value associated with the "station/grid" key from the configuration file.
 *
 * @return The value associated with the "station/grid" key as a string.
 */
std::string Configuration::getGrid()
{
	return getValue(f_grid).toString().toStdString();
}

/**
 * @brief Retrieves the value associated with the "station/lat" key from the configuration.
 *
 * This function retrieves the value associated with the "station/lat" key from the configuration file.
 *
 * @return The value associated with the "station/lat" key as a string.
 */
std::string Configuration::getLat()
{
	return getValue(f_lat).toString().toStdString();
}

/**
 * @brief Retrieves the value associated with the "station/lng" key from the configuration.
 *
 * This function retrieves the value associated with the "station/lng" key from the configuration file.
 *
 * @return The value associated with the "station/lng" key as a string.
 */
std::string Configuration::getLng()
{
	return getValue(f_lng).toString().toStdString();
}

/**
 * @brief Sets the username value in the configuration.
 *
 * This function sets the username value in the configuration.
 *
 * @param username The username value as a string.
 */
void Configuration::setUsername(const std::string& username)
{
	setValue(f_username, username.c_str());
}

/**
   * @brief Sets the password value in the configuration.
   *
   * This function sets the password value in the configuration by encrypting it and
   * storing it in the configuration file.
   *
   * @param password The password value as a string.
   */
void Configuration::setPassword(const std::string& password)
{
	try
	{
		std::string enc = encryptPassword(password);
		setValue(f_password, enc.c_str());
	}
	catch (Poco::IOException &e)
	{
		std::cout << e.message() << std::endl;
	}
}

/**
* @brief Sets the session key value in the configuration.
*
* This function sets the session key value in the configuration by updating
* the corresponding field in the configuration file.
*
* @param sessionKey The session key value as a string.
*/
void Configuration::setSessionKey(const std::string& sessionKey)
{
	setValue(f_sessionKey, sessionKey.c_str());
}

/**
* @brief Sets the session expiration value in the configuration.
*
* This function sets the session expiration value in the configuration by updating
* the corresponding field in the configuration file.
*
* @param sessionExpiration The session expiration value as a string.
*/
void Configuration::setSessionExpiration(const std::string& sessionExpiration)
{
	setValue(f_sessionExpiration, sessionExpiration.c_str());
}

/**
* @brief Sets the callsign for the station.
*
* This function sets the callsign for the station.
*
* @param grid The string representation of the callsign.
*/
void Configuration::setCallsign(const std::string& callsign)
{
	setValue(f_callsign, callsign.c_str());
}

/**
* @brief Sets the maidenhead grid for the station.
*
* This function sets the maidenhead grid for the station.
*
* @param grid The string representation of the maidenhead grid.
*/
void Configuration::setGrid(const std::string& grid)
{
	std::string origValue = getGrid();

	setValue(f_grid, grid.c_str());

	if(origValue != grid)
	{
		qDebug() << "Emitting grid change to " << grid;

		emit gridChanged(grid.c_str());
	}
}

/**
* @brief Sets the latitude for the station.
*
* This function sets the latitude for the station.
*
* @param grid The string representation of the latitude.
*/
void Configuration::setLat(const std::string& lat)
{
	std::string origValue = getLat();

	setValue(f_lat, lat.c_str());

	if(origValue != lat)
	{
		qDebug() << "Emitting lat change to " << lat;

		emit coordsChanged(std::stod(lat), std::stod(getLng()));
	}
}

/**
* @brief Sets the longitude for the station.
*
* This function sets the longitude for the station.
*
* @param grid The string representation of the longitude.
*/
void Configuration::setLng(const std::string& lng)
{
	std::string origValue = getLng();

	setValue(f_lng, lng.c_str());

	if(origValue != lng)
	{
		qDebug() << "Emitting lng change to " << lng;

		emit coordsChanged(std::stod(getLat()), std::stod(lng));
	}
}

/**
 * @brief Sets the value of the Js8CallEnabled configuration option.
 *
 * This function sets the value of the Js8CallEnabled configuration option
 * in the configuration file. The Js8CallEnabled option determines whether
 * Js8Call is enabled or disabled.
 *
 * @param enabled The value to set for the Js8CallEnabled option. True to enable,
 *                false to disable.
 */
void Configuration::setJs8CallEnabled(bool enabled)
{
	bool origJs8CallState = getJs8CallEnabled();

	setValue(f_js8CallEnable, enabled);

	if(origJs8CallState != enabled)
	{
		qDebug() << "Emitting JS8Call enabled state change to " << ((enabled) ? "true":"false");

		emit js8CallEnabledStateChange(enabled);
	}
}

/**
 * @brief Sets the host for the Js8Call application.
 *
 * This function allows you to set the network host for the Js8Call application.
 *
 * @param host The host address to set.
 */
/*void Configuration::setJs8CallHost(const std::string &host)
{
	setValue(f_js8CallHost, host.c_str());
}
 */

/**
 * @brief Sets the port for Js8Call communication.
 *
 * This function sets the port number to be used for communication with Js8Call software.
 *
 * @param port The port number to be used.
 */
/*void Configuration::setJs8CallPort(int port)
{
	setValue(f_js8CallPort, port);
}
 */

void Configuration::setJs8CallConnectionDetails(const std::string &host, int port)
{
	auto origHost = getJs8CallHost();
	auto origPort = getJs8CallPort();

	setValue(f_js8CallHost, host.c_str());
	setValue(f_js8CallPort, port);

	auto newHost = getJs8CallHost();
	auto newPort = getJs8CallPort();

	if(origHost != newHost || origPort != newPort)
	{
		emit connectionDetailsChanged(host.c_str(), port);
	}
}

/**
* @brief Checks if the configuration has a username value.
*
* This function checks if the configuration has a username value by calling the hasValue() function using the username key.
*
* @return True if the configuration has a username value, false otherwise.
*/
bool Configuration::hasUsername()
{
	return hasValue(f_username);
}

/**
* @brief Checks if the configuration has a password value.
*
* This function checks if the configuration has a password value by calling the hasValue() function using the password key.
*
* @return True if the configuration has a password value, false otherwise.
*/
bool Configuration::hasPassword()
{
	return hasValue(f_password);
}

/**
 * @brief Checks if the configuration has a session key value.
 *
 * This function checks if the configuration has a session key value by calling the hasValue() function using the session key key.
 *
 * @return True if the configuration has a session key value, false otherwise.
 */
bool Configuration::hasSessionKey()
{
	return hasValue(f_sessionKey);
}

/**
 * @brief Checks if the configuration has a session expiration value.
 *
 * This function checks if the configuration has a session expiration value by calling the hasValue() function using the session expiration key.
 *
 * @return True if the configuration has a session expiration value, false otherwise.
 */
bool Configuration::hasSessionExpiration()
{
	return hasValue(f_sessionExpiration);
}

/**
* @brief Check if a callsign value is available.
*
* This function checks whether a callsign value is available or not.
*
* @return true if a callsign value is available, false otherwise.
*/
bool Configuration::hasCallsign()
{
	return hasValue(f_callsign);
}

/**
* @brief Check if a maidenhead grid value is available.
*
* This function checks whether a maidenhead grid value is available or not.
*
* @return true if a maidenhead grid value is available, false otherwise.
*/
bool Configuration::hasGrid()
{
	return hasValue(f_grid);
}

/**
* @brief Check if a latitude value is available.
*
* This function checks whether a latitude value is available or not.
*
* @return true if a latitude value is available, false otherwise.
*/
bool Configuration::hasLat()
{
	return hasValue(f_lat);
}

/**
* @brief Check if a longitude value is available.
*
* This function checks whether a longitude value is available or not.
*
* @return true if a longitude value is available, false otherwise.
*/
bool Configuration::hasLng()
{
	return hasValue(f_lng);
}

/**
 * @brief Retrieves the value associated with the given key from the configuration.
 *
 * This function retrieves the value associated with the given key from the configuration file.
 *
 * @param key The key for which the value needs to be retrieved.
 * @return The value associated with the key as a string.
 */
QVariant Configuration::getValue(const std::string& key)
{
	return settings->value(key.c_str());
}

/**
 * @brief Sets the value associated with the given key in the configuration.
 *
 * This function sets the value associated with the given key in the configuration file.
 * If the key already exists in the configuration, the value will be updated. If the key does not exist,
 * a new key-value pair will be added to the configuration.
 *
 * @param key The key for which the value needs to be set.
 * @param value The value to be associated with the key.
 */
void Configuration::setValue(const std::string& key, const QVariant &value)
{
	settings->setValue(key, value);
}

/**
* @brief Checks if the configuration has a value associated with the given key.
*
* This function checks if the configuration has a value associated with the given key.
*c_str()}
* @param key The key for which the value needs to be checked.
* @return True if the configuration has a value associated with the key, false otherwise.
*/
bool Configuration::hasValue(const std::string& key)
{
	if(settings->contains(key))
	{
		return (!settings->value(key).toString().isEmpty());
	}

	return false;
}

/**
 * @brief Encrypts a password using AES-256-CBC encryption and base64 encoding.
 *
 * This function takes a password as input and encrypts it using AES-256-CBC encryption.
 * The encrypted password is then encoded in base64 format and returned as a string.
 *
 * @param password The password to be encrypted as a string.
 * @return The encrypted password as a string.
 */
std::string Configuration::encryptPassword(const std::string &password)
{
	Poco::Crypto::Cipher::Ptr cipher = createCipher();
	return cipher->encryptString(password, Poco::Crypto::Cipher::ENC_BASE64);
}

/**
 * @brief Decrypts an encrypted password.
 *
 * This function takes an encrypted password as input and decrypts it using AES-256-CBC decryption
 * and base64 decoding. The decrypted password is then returned as a string.
 *
 * @param encrypted The encrypted password to be decrypted as a string.
 * @return The decrypted password as a string.
 */
std::string Configuration::decryptPassword(const std::string &encrypted)
{
	Poco::Crypto::Cipher::Ptr cipher = createCipher();
	return cipher->decryptString(encrypted, Poco::Crypto::Cipher::ENC_BASE64);
}

/**
 * @brief Creates an instance of Poco::Crypto::Cipher.
 *
 * This function creates an instance of Poco::Crypto::Cipher with the specified encryption algorithm, password key, and initialization vector.
 *
 * @return A pointer to the created instance of Poco::Crypto::Cipher.
 */
Poco::Crypto::Cipher::Ptr Configuration::createCipher()
{
	std::string passkey = createCryptoPassKey();
	std::string ivStr = ivStr_.substr(0, 32);

	Poco::Crypto::Cipher::ByteVec iv{ivStr.begin(), ivStr.end()};
	Poco::Crypto::Cipher::ByteVec passwordKey{ passkey.begin(), passkey.end() };
	Poco::Crypto::CipherKey key("aes-256-cbc", passwordKey, iv);
	Poco::Crypto::Cipher::Ptr cipher = Poco::Crypto::CipherFactory::defaultFactory().createCipher(key);

	return cipher;
}

/**
 * @brief Creates a passkey for encryption/decryption.
 *
 * This function creates a passkey for encryption/decryption by concatenating the username string multiple times.
 * The passkey is then truncated to a length of 64 characters before being returned.
 *
 * @return The passkey for decryption as a string.
 */
std::string Configuration::createCryptoPassKey()
{
	std::string passkey = getUsername();
	while(passkey.size() < 64) {
		passkey.append(getUsername());
	}
	return passkey.substr(0, 64);
}

/**
 * Initialization vector for the cipher
 * Lame attempt at security by obscurity
 */
const std::string Configuration::ivStr_ = "https://xmldata.qrz.com/xml/current/?username=%username%;password=%password%";


#ifndef QRZ_CONFIGURATION_H
#define QRZ_CONFIGURATION_H

#include <string>

#include <Poco/AutoPtr.h>
#include <Poco/Crypto/Cipher.h>
#include <QSettings>

namespace qrz
{
	class Configuration : public QObject
	{
		Q_OBJECT
	public:
		Configuration();
		~Configuration();

		/**
		 * @brief Returns the callsign value from the configuration.
		 *
		 * This function retrieves the callsign value from the configuration.
		 *
		 * @return The callsign value as a string.
		 */
		std::string getUsername();

		/**
		 * @brief Retrieves the password value from the configuration.
		 *
		 * This function retrieves the password value from the configuration file.
		 *
		 * @return The password value as a string.
		 */
		std::string getPassword();

		/**
		 * @brief Retrieves the session key value from the configuration.
		 *
		 * This function retrieves the session key value from the configuration file.
		 *
		 * @return The session key value as a string.
		 */
		std::string getSessionKey();

		/**
		 * @brief Retrieves the session expiration value from the configuration file.
		 *
		 * This function retrieves the session expiration value from the configuration
		 * file using the key "session_expiration". Session expiration represents the
		 * duration of time in which a session remains valid.
		 *
		 * @return The session expiration value as a string.
		 */
		std::string getSessionExpiration();

		/**
		 * @brief Retrieves the value of the Js8CallEnable configuration option.
		 *
		 * This function retrieves the value of the Js8CallEnable configuration option from the configuration file.
		 *
		 * @return The value of the Js8CallEnable configuration option as a boolean.
		 */
		bool getJs8CallEnabled();

		/**
		 * @brief Retrieves the value associated with the "js8call/host" key from the configuration.
		 *
		 * This function retrieves the value associated with the "js8call/host" key from the configuration file.
		 *
		 * @return The value associated with the "js8call/host" key as a string.
		 */
		std::string getJs8CallHost();

		/**
		 * @brief Retrieves the value associated with the "station/callsign" key from the configuration.
		 *
		 * This function retrieves the value associated with the "station/callsign" key from the configuration file.
		 *
		 * @return The value associated with the "station/callsign" key as a string.
		 */
		std::string getCallsign();

		/**
		* @brief Retrieves the value associated with the "station/grid" key from the configuration.
		*
		* This function retrieves the value associated with the "station/grid" key from the configuration file.
		*
		* @return The value associated with the "station/grid" key as a string.
		*/
		std::string getGrid();

		/**
		* @brief Retrieves the value associated with the "station/lat" key from the configuration.
		*
		* This function retrieves the value associated with the "station/lat" key from the configuration file.
		*
		* @return The value associated with the "station/lat" key as a string.
		*/
		std::string getLat();

		/**
		* @brief Retrieves the value associated with the "station/lng" key from the configuration.
		*
		* This function retrieves the value associated with the "station/lng" key from the configuration file.
		*
		* @return The value associated with the "station/lng" key as a string.
		*/
		std::string getLng();

		/**
		 * @brief Retrieves the JS8Call port value from the configuration.
		 *
		 * This function retrieves the JS8Call port value from the configuration file.
		 *
		 * @return The JS8Call port value as an integer.
		 */
		int getJs8CallPort();

		/**
		 * @brief Sets the username value in the configuration.
		 *
		 * This function sets the username value in the configuration.
		 *
		 * @param username The username value as a string.
		 */
		void setUsername(const std::string &username);

		/**
		 * @brief Sets the password value in the configuration.
		 *
		 * This function sets the password value in the configuration by encrypting it and
		 * storing it in the configuration file.
		 *
		 * @param password The password value as a string.
		 */
		void setPassword(const std::string &password);

		/**
		 * @brief Sets the session key value in the configuration.
		 *
		 * This function sets the session key value in the configuration by updating
		 * the corresponding field in the configuration file.
		 *
		 * @param sessionKey The session key value as a string.
		 */
		void setSessionKey(const std::string &sessionKey);

		/**
		 * @brief Sets the session expiration value in the configuration.
		 *
		 * This function sets the session expiration value in the configuration by updating
		 * the corresponding field in the configuration file.
		 *
		 * @param sessionExpiration The session expiration value as a string.
		 */
		void setSessionExpiration(const std::string &sessionExpiration);

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
		void setJs8CallEnabled(bool enabled);

		/**
		 * @brief Sets the host for the Js8Call application.
		 *
		 * This function allows you to set the network host for the Js8Call application.
		 *
		 * @param host The host address to set.
		 */
		//void setJs8CallHost(const std::string &host);

		/**
		 * @brief Sets the port for Js8Call communication.
		 *
		 * This function sets the port number to be used for communication with Js8Call software.
		 *
		 * @param port The port number to be used.
		 */
		//void setJs8CallPort(int port);

		void setJs8CallConnectionDetails(const std::string &host, int port);

		/**
		 * @brief Sets the callsign for the station.
		 *
		 * This function sets the callsign for the station.
		 *
		 * @param grid The string representation of the callsign.
		 */
		void setCallsign(const std::string &callsign);

		/**
		 * @brief Sets the maidenhead grid for the station.
		 *
		 * This function sets the maidenhead grid for the station.
		 *
		 * @param grid The string representation of the maidenhead grid.
		 */
		void setGrid(const std::string &grid);

		/**
		 * @brief Sets the latitude for the station.
		 *
		 * This function sets the latitude for the station.
		 *
		 * @param grid The string representation of the latitude.
		 */
		void setLat(const std::string &lat);

		/**
		 * @brief Sets the lng for the station.
		 *
		 * This function sets the lng for the station.
		 *
		 * @param grid The string representation of the lng.
		 */
		void setLng(const std::string &lng);

		/**
		 * @brief Checks if the configuration has a username value.
		 *
		 * This function checks if the configuration has a username value.
		 *
		 * @return True if the configuration has a username value, false otherwise.
		 */
		bool hasUsername();

		/**
		* @brief Checks if the configuration has a password value.
		*
		* This function checks if the configuration has a password value.
		*
		* @return True if the configuration has a password value, false otherwise.
		*/
		bool hasPassword();

		/**
		 * @brief Checks if the configuration has a session key value.
		 *
		 * This function checks if the configuration has a session key value.
		 *
		 * @return True if the configuration has a session key value, false otherwise.
		 */
		bool hasSessionKey();

		/**
		 * @brief Checks if the configuration has a session expiration value.
		 *
		 * This function checks if the configuration has a session expiration value.
		 *
		 * @return True if the configuration has a session expiration value, false otherwise.
		 */
		bool hasSessionExpiration();

		/**
		 * @brief Checks if the Js8Call feature is enabled.
		 *
		 * @return true if Js8Call is enabled, false otherwise.
		 */
		bool hasJs8CallEnabled();

		/**
		 * @brief Check if the application has Js8CallHost.
		 *
		 * This function checks whether the application has Js8CallHost installed.
		 *
		 * @return true if Js8CallHost is installed, false otherwise.
		 */
		bool hasJs8CallHost();

		/**
		 * @brief Checks if the device has a JS8 call port.
		 *
		 * This function determines if the device has a JS8 call port or not.
		 *
		 * @return True if the device has a JS8 call port, false otherwise.
		 */
		bool hasJs8CallPort();

		/**
		 * @brief Check if a maidenhead grid value is available.
		 *
		 * This function checks whether a maidenhead grid value is available or not.
		 *
		 * @return true if a maidenhead grid value is available, false otherwise.
		 */
		bool hasGrid();

		/**
		 * @brief Check if a callsign value is available.
		 *
		 * This function checks whether a callsign value is available or not.
		 *
		 * @return true if a callsign value is available, false otherwise.
		 */
		bool hasCallsign();

		/**
		 * @brief Check if a latitude value is available.
		 *
		 * This function checks whether a latitude value is available or not.
		 *
		 * @return true if a latitude value is available, false otherwise.
		 */
		bool hasLat();

		/**
		 * @brief Check if a longitude value is available.
		 *
		 * This function checks whether a longitude value is available or not.
		 *
		 * @return true if a longitude value is available, false otherwise.
		 */
		bool hasLng();

		/**
		 * @brief Retrieves the value associated with the given key from the configuration.
		 *
		 * This function retrieves the value associated with the given key from the configuration file.
		 *
		 * @param key The key for which the value needs to be retrieved.
		 * @return The value associated with the key as a string.
		 */
		QVariant getValue(const std::string &key);

		/**
		 * @brief Sets the value associated with the given key in the configuration.
		 *
		 * This function sets the value associated with the given key in the configuration file.
		 *
		 * @param key The key for which the value needs to be set.
		 * @param value The value to be associated with the key.
		 */
		void setValue(const std::string &key, const QVariant &value);

		/**
		 * @brief Checks if the configuration has a value associated with the given key.
		 *
		 * This function checks if the configuration has a value associated with the given key.
		 *
		 * @param key The key for which the value needs to be checked.
		 * @return True if the configuration has a value associated with the key, false otherwise.
		 */
		bool hasValue(const std::string &key);

	signals:
		void js8CallEnabledStateChange(bool);
		void connectionDetailsChanged(const QString &hostName, quint16 port);
		void gridChanged(const QString &grid);
		void coordsChanged(double lat, double lng);
	private:
		// Field names
		static inline const char *f_username = "authentication/username";
		static inline const char *f_password = "authentication/password";
		static inline const char *f_sessionKey = "session/key";
		static inline const char *f_sessionExpiration = "session/expiration";
		static inline const char *f_js8CallEnable = "js8call/enable";
		static inline const char *f_js8CallHost = "js8call/host";
		static inline const char *f_js8CallPort = "js8call/port";
		static inline const char *f_callsign = "station/callsign";
		static inline const char *f_grid = "station/grid";
		static inline const char *f_lat = "station/lat";
		static inline const char *f_lng = "station/lng";

		QSettings *settings;

		/**
		 * @brief Encrypts a password using AES-256-CBC encryption and base64 encoding.
		 *
		 * This function takes a password as input and encrypts it using AES-256-CBC encryption.
		 * The encrypted password is then encoded in base64 format and returned as a string.
		 *
		 * @param password The password to be encrypted as a string.
		 * @return The encrypted password as a string.
		 */
		std::string encryptPassword(const std::string &password);

		/**
		 * @brief Decrypts an encrypted password.
		 *
		 * This function takes an encrypted password as input and decrypts it using AES-256-CBC decryption
		 * and base64 decoding. The decrypted password is then returned as a string.
		 *
		 * @param encrypted The encrypted password to be decrypted as a string.
		 * @return The decrypted password as a string.
		 */
		std::string decryptPassword(const std::string &encrypted);

		/**
		 * @brief Creates an instance of Poco::Crypto::Cipher.
		 *
		 * This function creates an instance of Poco::Crypto::Cipher with the appropriate configuration
		 *
		 * @return A pointer to the created instance of Poco::Crypto::Cipher.
		 */
		Poco::AutoPtr<Poco::Crypto::Cipher> createCipher();

		/**
		 * @brief Creates a passkey for encryption/decryption.
		 *
		 * This function creates a passkey for encryption/decryption
		 *
		 * @return The passkey for decryption as a string.
		 */
		std::string createCryptoPassKey();

		// String to use as the initialization vector for password encryption
		static const std::string ivStr_;
	};
}

#endif //QRZ_CONFIGURATION_H

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "webserv.hpp"
# include "Config.hpp"

class Request
{
	private:
	//string with the method of the request
		std::string _methodString;
	//string with the path of the request
		std::string _pathString;
	//string with the version of the request
		std::string _versionString;
	//vector with the headers of the request
		std::vector<std::string> _headersVector;
	//map with headers as key and header's value as value
		std::map<std::string, std::string> _headersMap;
	//string with the body of the request
		std::string _bodyString;
		int _flag;
		t_configParams _configParams;
		std::string _startPathLocation;
		std::string _endPathLocation;
		std::vector<t_locations> _configParamsLocations;
		t_locations _currentLocation;
		std::map<int, std::pair<std::string, int> > _currentErrorPages;
		std::string _finalUploadPathIfAllowed;
		bool _isLocationSet;
		std::string _httpRequest;

	

	public:

		Request();
		~Request();
		Request(const Request &src);
		Request &operator=(const Request &src);

		//setters
		void setMethodString(std::string methodString);
		void setPathString(std::string pathString);
		void setVersionString(std::string versionString);
		void setHeadersVector(std::vector<std::string> headersVector);
		void setHeadersMap(std::map<std::string, std::string> headersMap);
		void setBodyString(std::string bodyString);
		void setFlag(int flag);
		void setConfigParamsLocations(std::vector<t_locations> configParamsLocations);
		void setStartPathLocation(std::string startPathLocation);
		void setEndPathLocation(std::string endPathLocation);
		void setCurrentLocation(t_locations currentLocation);
		void setCurrentErrorPages(std::map<int, std::pair<std::string, int> > currentErrorPages);
		void setIsLocationSet(bool isLocationSet);
		void setCurrentLocUploadFilePath(std::string uploadFilePath);
		void setFinalUploadPathIfAllowed(std::string finalUploadPathIfAllowed);


		//getters
		std::string getMethodString()const;
		std::string getPathString()const;
		std::string getVersionString()const;
		std::vector<std::string> getHeadersVector() const;
		std::map<std::string, std::string> getHeadersMap() const;
		std::string getBodyString() const;
		t_configParams getConfigParamsStruct() const;
		std::vector<t_locations> getConfigParamsLocations() const;
		std::string getStartPathLocation() const;
		std::string getEndPathLocation() const;
		t_locations getCurrentLocation() const;
		std::string getCurrentLocUploadFilePath() const;
		std::string getFinalUploadPathIfAllowed() const;
		std::map<int, std::pair<std::string, int> > getCurrentErrorPages() const;
		bool getIsLocationSet() const;
	    int getFlag() const;

		// parsing requete
		void parseFromString(const std::string& rawData, t_configParams &configParams);
		bool isChunkedRequest( void );
		void clear();

};




#endif
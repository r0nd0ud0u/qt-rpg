
#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "IniTools.h"
#include "AbstractIniFile.h"
#include "IniFileReadWrite.h"
#include <string>
#include <vector>
#include <tuple>


//forward-declaration
class Parameters;

class ParametersSection
{
public:
    enum tTriStateBool { tTriStateBool_False = -1, tTriStateBool_Default = 0, tTriStateBool_True = 1 };
    
public:
    void RegisterParamRead(const std::string& strParamNameReal, const std::string& strValue, bool bIsValid, Parameters& oparam);

    bool DecodeStr(const AbstractIniFile& iniFile, const std::string& strParamNameReal, std::string& outVal, Parameters& oparam);
    bool DecodePwd(const AbstractIniFile& iniFile, const std::string& strParamNameReal, std::string& outVal, Parameters& oparam);
    bool DecodeBool(const AbstractIniFile& iniFile, const std::string& strParamNameReal, bool& outVal, Parameters& oparam);
    bool DecodeTriStateBool(const AbstractIniFile& iniFile, const std::string& strParamNameReal, tTriStateBool& outVal, Parameters& oparam);
    bool DecodeInt(const AbstractIniFile& iniFile, const std::string& strParamNameReal, int& outVal, int base, Parameters& oparam);
    bool DecodeLong(const AbstractIniFile& iniFile, const std::string& strParamNameReal, int64_t& outVal, int base, Parameters& oparam);
    bool DecodeDouble(const AbstractIniFile& iniFile, const std::string& strParamNameReal, double& outVal, Parameters& oparam);

    // get list of incorrect keys, sections and values
public:
    std::string m_strSectionNameAbstract; // section name, maybe with %
    std::string m_strSectionNameReal; // section name as read in file (without %)
    std::vector<std::tuple<std::string, std::string>> m_listIncorrectValues; // tuple : <key, value>
};

class Parameters_GLOBAL : public ParametersSection
{
public:
    /* LOG_PATH =  */
    std::string m_LOG_PATH = "log";

    /* SKYOPSGATEWAY_TCPSERVER_ADDRESS =  */
    std::string m_SKYOPSGATEWAY_TCPSERVER_ADDRESS = "127.0.0.1";

    /* SKYOPSGATEWAY_TCPSERVER_PORT =  */
    std::string m_SKYOPSGATEWAY_TCPSERVER_PORT = "36504";

    /* IS_WINDOW_SHOWN =  */
    bool m_IS_WINDOW_SHOWN = false;

    /* DEBUG_MODE =  */
    bool m_DEBUG_MODE = false;
};

class Parameters
{
    friend class ParametersSection;
    typedef std::string sec;
    typedef std::string key;
    typedef std::string val;
    typedef std::tuple<sec, key> secKey;
    typedef std::tuple<sec, key, val> secKeyVal;
    typedef std::tuple<sec, key, key, bool> secKeyLegacyBoth; // tuple : <key_old, key_new, are_both_present>

public:
    Parameters_GLOBAL GLOBAL;

    static Parameters ReadConfigFile(const AbstractIniFile& iniFile);


    // get list of incorrect keys, sections and values
public:
    std::vector<sec> GetListUnexpectedSections() const;
    std::vector<secKey> GetListUnexpectedKeys() const;
    std::vector<secKeyVal> GetListIncorrectValues() const;
    std::vector<secKeyLegacyBoth> GetListLegacyParams() const;
    void RegisterIncorrectValue(const sec&, const key&, const val&);
    void RegisterFoundExpectedSection(const sec& strSection);
    void RegisterLegacyParam(const sec&, const key&, const key&, const bool&);

    // private attributes for incorrect keys, sections and values
private:
    static const std::vector<sec> m_glistPossibleSections;
    std::vector<sec> m_listFoundExpectedSections;
    std::vector<std::tuple<sec, std::vector<key>>> m_listAllSectionsReadInFile; // list of tuples : (section name, section keys)
    std::vector<secKey> m_listReadExpectedParameters;
    std::vector<secKeyVal> m_listIncorrectValues;
    std::vector<secKeyLegacyBoth> m_listLegacyParams;
};

#endif // PARAMETERS_H

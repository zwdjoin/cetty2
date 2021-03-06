#if !defined(CETTY_SHIRO_REALM_SQLREALM_H)
#define CETTY_SHIRO_REALM_SQLREALM_H

#include <cetty/shiro/realm/AuthorizingRealm.h>
#include <cetty/shiro/realm/SqlRealmConfig.h>

namespace cetty {
namespace shiro {
namespace realm {

class SqlRealm : public AuthorizingRealm {
public:
    enum SaltStyle {NO_SALT, CRYPT, COLUMN, EXTERNAL};

    static const std::string NAME;

    static const std::string DEFAULT_AUTHENTICATION_QUERY;
    static const std::string DEFAULT_SALTED_AUTHENTICATION_QUERY;
    static const std::string DEFAULT_USER_ROLES_QUERY;
    static const std::string DEFAULT_PERMISSIONS_QUERY;

public:
    SqlRealm();

    virtual ~SqlRealm(){}

    virtual void doGetAuthenticationInfo(const AuthenticationToken& token,
                                       const GetAuthenticationInfoCallback& callback);

    virtual void doGetAuthorizationInfo(const PrincipalCollection& principals,
                                        const GetAuthorizationInfoCallback& callback);

    virtual void onLogout(const std::string &principal){
        AuthorizingRealm::onLogout(principal);
    }

protected:
    SqlRealmConfig config;

private:
    void init();

    void getPasswordForUser(const std::string &username, std::vector<std::string> *passwd);
    void getRoleNamesForUser(const std::string &username, std::vector<std::string> *roles);
    void getPermissions(const std::string &userName,
                        const std::vector<std::string> &roles,
                        std::vector<std::string> *permissions);
    void getSaltForUser(const std::string &userName, std::string *salt){}

};

}
}
}

#endif // #if !defined(CETTY_SHIRO_REALM_SQLREALM_H)

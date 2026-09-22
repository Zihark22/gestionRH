#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#define APP_VERSION "1.2.0"
#define CONFIG_FILE_NAME "config.ini"
#define SERVER_GROUP_NAME "Server"
#define PORT_KEY "port"
#define HOST_KEY "host"
#define DEFAULT_HOST "127.0.0.1"
#define DEFAULT_PORT 8080
#define STYLE_FILE_NAME ":/style.qss"

enum class ScreenId {
    Home,
    TableViewer,
    Error
};

#endif // PARAMETERS_HPP

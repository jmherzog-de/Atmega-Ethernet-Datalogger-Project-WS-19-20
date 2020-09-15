#ifndef IP_CONFIG_H_
#define IP_CONFIG_H_

#undef NTP_client

#define UDP_client

#undef UDP_server

#undef WWW_server

#undef PING_client
#define PINGPATTERN 0x42

#undef WOL_client

#undef GRATARP

#define WWW_client

// functions to decode cgi-form data
#undef FROMDECODE_webserv_help

// functions to encode a URL (mostly needed for a web client)
#define URLENCODE_webserv_help

#endif /* IPCONFIG_H_ */
#ifndef __LRB_TCPCONFIG_H
#define __LRB_TCPCONFIG_H

namespace lrb {

		class tcpConfig {
				public:
				tcpConfig(tcpConfig *config) : _host(config->host()), _port(config->port()), _timeout(config->timeout()), _maxretry(config->maxRetry()) {};
				tcpConfig(const char *host, const int port) : _host(host), _port(port), _timeout(10), _maxretry(3) {};
				~tcpConfig() {};

				private:
				const char *_host;
				const int _port;
				int _timeout;
				int _retry;
				int _maxretry;

				public:
				void setTimeout(int timeout) {this->_timeout = timeout;};
				void setRetry(int retry) {this->_retry = retry;};
				void setMaxretry(int max) {this->_maxretry = max;};

				const char *host() {return this->_host;};
				const int port() {return this->_port;};
				int timeout() {return this->_timeout;};
				int retry() {return this->_retry;};
				int maxRetry() {return this->_maxretry;};

				static tcpConfig *defaultConfig() {return new tcpConfig("127.0.0.1", 8080);};
		};
}


#endif

#include "get_web_object.h"

int main(int argc, char *argv[]) {

	char *request = "GET /tutorials/other/top-20-mysql-best-practices/ HTTP/1.1\nHost: net.tutsplus.com\nUser-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) Gecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729)\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\nAccept-Language: en-us,en;q=0.5\nAccept-Encoding: gzip,deflate\nAccept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\nKeep-Alive: 300\nConnection: keep-alive\nCookie: PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120\nPragma: no-cache\nCache-Control: no-cache";	

	char *pragma = parseRequest(request, "Pragma");
	printf("Pragma: %s\n", pragma);
	char *host = parseRequest(request, "Host");
	printf("Host: %s\n", host);
	return 0;
}

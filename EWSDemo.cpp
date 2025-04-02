/* Copyright 2016 Forrest Heller. Released under the 2-clause BSD license in EmbeddableWebServer.h */

/* This is a demo for the Embeddable Web Server. See EmbedabbleWebServer.h and the README for smaller
examples. EWS code in general is written for Linux and Mac OS X and puts the burden of emulation on
Windows, rather than emulating Linux/OS X interfaces. */

#pragma warning(disable:4244)
#pragma warning(disable:4267)
#include "EmbeddableWebServer.h"

#pragma comment(lib, "ws2_32")

static struct Server server = {0};

int main(int argc, const char * argv[]) {
	printf("Server initializing...\n");
	serverInit(&server);
	printf("Server initialized.\n");
	acceptConnectionsUntilStoppedFromEverywhereIPv4(&server, 80);
	printf("Server shutting down...\n");
	serverDeInit(&server);
	printf("Server shut down.\n");
	return 0;
}

struct Response* createResponseForRequest(const struct Request* request, struct Connection* connection) {
	if (strcmp(request->path, "/") == 0)
	{
		struct HeapString connectionDebugInfo = connectionDebugStringCreate(connection);
		struct Response* response = responseAllocWithFormat(200, "OK", "text/html; charset=UTF-8",
			"<html><head><title>Embedded C Web Server by Forrest Heller</title></head>"
			"<body>"
			"<h2>Version %s</h2>"
			"<h2>Server Status</h2>"
			"<table border=\"1\">\n"
			"<tr><td>Active connections</td><td>%" PRId64 "</td></tr>\n"
			"<tr><td>Total connections</td><td>%" PRId64 "</td></tr>\n"
			"<tr><td>Total bytes sent</td><td>%" PRId64 "</td></tr>\n"
			"<tr><td>Total bytes received</td><td>%" PRId64 "</td></tr>\n"
			"<tr><td>Heap string allocations</td><td>%" PRId64 "</td></tr>\n"
			"<tr><td>Heap string reallocations</td><td>%" PRId64 "</td></tr>\n"
			"<tr><td>Heap string frees</td><td>%" PRId64 "</td></tr>\n"
			"<tr><td>Heap string total bytes allocated</td><td>%" PRId64 "</td></tr>\n"
			"</table>"
			"<h2>Connection Debug Info</h2><pre>%s</pre>"
			"</body></html>",
			EMBEDDABLE_WEB_SERVER_VERSION_STRING,
			counters.activeConnections,
			counters.totalConnections,
			counters.bytesSent,
			counters.bytesReceived,
			counters.heapStringAllocations,
			counters.heapStringReallocations,
			counters.heapStringFrees,
			counters.heapStringTotalBytesReallocated,
			connectionDebugInfo.contents);
		heapStringFreeContents(&connectionDebugInfo);
		return response;
	}
	return responseAllocServeFileFromRequestPath("/", request->path, request->pathDecoded, "EWSDemoFiles");
}

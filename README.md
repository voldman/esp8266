We designed a custom Teensy library for this class to interface with the ESP8266 chip.  This library can be used to send non-blocking HTTP requests that use a very low fraction of the Teensy's clock cycles.

# Basic Examples
There are two examples bundled with the library (in the Github repo).

In the "blocking" example, we make a request.  The `sendRequest()` function does not wait for a response, so if we want to wait we have to do so explicitly with a `while` loop.  It's a good idea to limit how long you're willing to wait for a response, because requests sometimes fail and we don't want to wait forever for a response that will never arrive.

In the "nonblocking" example, we don't do any waiting at all.  Every 8 seconds, we send a request.  Each loop, we also start by checking if the wifi library has a response from a previously sent request.  It's a bit more complicated to follow logically, but we get the benefit of using extremely little time to do our Wifi operations.


# Limitations

* The library can only send requests up to 2KB in size, and it can only handle responses up to 4KB in size.  For larger requests, you can use `sendBigRequest`, but that's a new feature and it's takes a bit of care to use.
* The library can only have one request "in flight" at a time.  If the user tries to make a request while one is already in flight, it is ignored.
* The library doesn't handle some of the ESP8266 failure scenarios, requiring a reset of the ESP8266.  Unless you've connected a wire to the ESP8266's "reset" pin, this requires you to power-cycle your system.
* The library only recognizes responses beginning with "<http>" and ending with "</http>", a pretty big limitation.

# Common Problems

**Requests don't receive a response:**
	* Check the serial monitor.  Is the request being sent out at all?  If not, your ESP8266 make need to be reset. 
	* Is the request being sent but no response is coming back?  That would seem to be a server-side issue
	* Is a response coming back but your code gets stuck anyway?  Perhaps you're getting stuck in a loop in your code.

**Making requests too close together can be an issue.**  If you make a request very soon after the previous response arrives, the library sometimes fails.

**You see a warning: "inputBuffer full"**.  This means the HTTP response is too big for our library to handle (> 4KB).  You can go into the library and edit both `BUFFERSIZE` and `RESPONSESIZE` if you need this to be a bit bigger.

**You just see "CIPSTATUS time out" over and over**.  This means that the library is repeatedly asking the ESP8266 for its status, and receiving no response.  This usually means you have to reset your ESP8266.

With any issue, be sure to initialize the library with the "verbose" argument set to `true`, so that you get useful informating about the internal operation of the library.

# Documentation of public methods

### ESP8266(bool verboseSerial)

* Initializes the ESP8266 class.

* Library will produce verbose outputs over USB serial if and only if `verboseSerial==true`.

* Argument is optional, and defaults to `true`.

### void begin()

* Checks for ESP8266 connection and resets the ESP8266.

* Call this before any other functions.

### void connectWifi(String ssid, String password)

* Attempts to connect to a network with the given SSID, using the given password.

* If the network is not password protected, password should be the empty string ("").

* Times out after 15 seconds.

### bool isConnected()

* Returns `true` if ESP8266 was connected to a network at the time of the last status check, and `false` otherwise.  Status checks occur every 10 seconds.

### void sendRequest(int type, String domain, int port, String path, String data, bool auto_retry)

* Sends an HTTP request directed at the given domain, port, and path.

* For the first argument, use "GET" and "POST" (without the quotes), which are macros defined in the library.

* Put GET parameters or POST data in the `data` input.  Do not attempt to put GET parameters in the `path` input.

* This function exits after the request is sent, and it does <strong>not</strong> wait for a response.

* The `auto_retry` flag is optional and defaults to `false`.  If `auto_retry` is `true`, the library will repeatedly attempt this request until an HTML response is received.

* Times out after roughly 15 seconds, though this will be shortened in later versions.

* If a request is already in progress, this function does nothing.

### void clearRequest()

* Clears the current request

* The main use case is to cease the repeated requests that occur when `sendRequest()` is called with `auto_retry==true`.

### bool hasResponse())

* Returns `true` if the library has received a valid HTML response since the last call to `sendRequest()`, and false otherwise.

* Note that the current version of the library only works with HTML responses.

### String getResponse()

* Returns the current HTML response as an Arduino `String`.

* You should check that `hasResponse()==true` before calling this.

* After calling this, the response is "cleared out" of the library.  That is to say, immediately after `getResponse()` is called, `hasResponse()` will return `false`.

### bool isBusy()

* Returns `true` if there's is currently a request "in flight", and `false` otherwise.

### bool reset()

* Sends the following commands to the ESP8266: "AT+CWMODE_DEF=1", "AT+CWAUTOCONN=0", "AT+RST".

* Returns `true` if this operation was successful.

* Unlike request-sending or connecting to a network, this is a blocking operation.

### bool restore()

* Sends the command "AT+RESTORE" to the ESP8266, and then calls `reset()`. 

* Returns `true` if this operation was succesful.

* Unlike request-sending or connection to a network, this is a blocking operation.

### String getMAC()

* Returns the MAC address of the ESP8266

### String sendCustomCommand(String command, unsigned long timeout)

* Sends the given command to the ESP8266 over serial.

* Blocks for `timeout` milliseconds, and returns any result that has come back from the ESP8266 during this time.

* Unlike request-sending or connection to a network, this is a blocking operation.

### bool isAutoConn()

* Returns `true` if the system is periodically checking for a network connection, and attempting reconnection if the connection is lost.  This behavior is enabled by default.

### void setAutoConn(bool value)

* Turns the "autoconnection" feature on if `value==true` and turns it off otherwise.

### int getTransmitCount()

* Returns the number of HTTP requests transmitted by the ESP8266 chip.

### void resetTransmitCount()

* Resets the transmit count to 0.

### int getReceiveCount()

* Returns the number of valid HTML messages received by the ESP8266 chip.

### void resetReceiveCount()

* Resets the receive count to 0.

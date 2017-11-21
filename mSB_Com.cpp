// Change to C++ Class?


// compare json tokens
static int jsoneq(const char *, jsmntok_t *, const char *);
static int jsonGetStringValue(const char *, const int, jsmntok_t *, const char *, char *);
static int jsonGetIntegerValue(const char *, const int, jsmntok_t *, const char *, int *);



/*
    Function to serialize sign in message from node data to mSB.

    buffer:     holding result of serialization
    bufferLen:  max len of result
    macAddress: MAC address of device, must be registred on mSB.com
    ipAddress:  IP-Address of device, will show in node status om mSB.com
    orgID:      Organisation ID to connect device on mSB.com
    plattform:  Plattform the device is running ex. ARM
    target:     Device type ex. K64F 

*/

int mSB_Serialize_signInMessage(unsigned char* buffer, unsigned int bufferLen, const unsigned char* macAddress, const unsigned char* ipAddress,
                             const unsigned char* orgID, const unsigned char* plattform, const unsigned char* target)
{
    int len = 0;

    // Create json formatted sign in message to msb.com
    len = snprintf(buffer, bufferLen, "signIn::{\"macAddress\": \"%s\",\"ip\": \"%s\",\"organizationId\": \"%s\", \"platform\": \"%s\", \"board\": \"%s\"}",
            macAddress, ipAddress, orgID, plattform, target);
    if( len == bufferLen )
    {
        // Buffer is to small for the signIn message
        return -1;
    }

    return len;
}

/*
    Function to deserialize JSON sign in response from mSB.com.
    Result is placed in Msb struct

    buffer:     message read from network
    bufferLen:  Length of buffer read from network
    msb:        Msb struct holding information from mSB.com for ex. IoT HUB

*/
int mSB_Deserialize_signInMessage(unsigned char* buffer, unsigned int bufferLen, struct Msb *msb )
{
    // ----------JSON------------
    int numTokens = 0;
    jsmn_parser jsonParser;
    jsmntok_t jsonTokens[NUMBER_OF_JSON_TOKENS]; // Create memmory space for JSON tokens
    jsmn_init(&jsonParser);

    // Get start and length of json from sign in response
    unsigned char* jsonStart = memchr( buffer, '{', bufferLen );
    int jsonLen = bufferLen - (jsonStart - buffer);

    // Parse msb node information from json response
    numTokens = jsmn_parse(&jsonParser, jsonStart, jsonLen, jsonTokens, sizeof(jsonTokens) / sizeof(jsonTokens[0]));
    
    if( numTokens < 0 )
    {
        // Error parsing json, bad sign in message
        return -1;
    }
    else{

        // Get info from parsed data
        jsonGetToken(jsonStart, numTokens, jsonTokens, "binaryUri", msb->binaryUri);
        jsonGetToken(jsonStart, numTokens, jsonTokens, "binaryVersion", msb->binaryVersion);
        jsonGetToken(jsonStart, numTokens, jsonTokens, "name", msb->name);
        jsonGetToken(jsonStart, numTokens, jsonTokens, "hubProvider", msb->hubProvider);
        jsonGetToken(jsonStart, numTokens, jsonTokens, "state", msb->state);
        // TO DO: Get debug as bool, jsonGetToken(jsonStart, numTokens, jsonTokens, "debug", msb->debug);

    
        if(strcmp(msb.hubProvider, "AZUREIOT") == 0)
        {
            // Parse Azure Hub settings from json
            jsonGetToken(jsonStart, numTokens, jsonTokens, "securityToken", msb->azure.securityToken);
            jsonGetToken(jsonStart, numTokens, jsonTokens, "senderToken", msb->azure.senderToken);
            jsonGetToken(jsonStart, numTokens, jsonTokens, "receiverToken", msb->azure.receiverToken);
            jsonGetToken(jsonStart, numTokens, jsonTokens, "sbNamespace", msb.azure.sbNamespace);
            // TO DO: Get lifetime as int, jsonGetToken(jsonStart, numTokens, jsonTokens, "tokenLifeTime", msb->azure.tokenLifeTime);
            jsonGetToken(jsonStart, numTokens, jsonTokens, "trackingHubName", msb->azure.trackingHubName);
            jsonGetToken(jsonStart, numTokens, jsonTokens, "trackingToken", msb->azure.trackingToken);
            jsonGetToken(jsonStart, numTokens, jsonTokens, "protocol", msb->azure.protocol);
            //TODO Get port as int from json
        }
        else if( strcmp(msb.hubProvider, "???AWS???") == 0 )
        {
            //TO DO Parse AWS response
        }
        else if( strcmp(msb.hubProvider, "???IBM???") == 0 )
        {
            // TO DO Parse IBN response
        }
    }

 

}

/*
    Function to serialize JSON ping to mSB.com.
    
    buffer:     message to write to network
    bufferLen:  Length of buffer write to network
    msb:        Msb struct holding information from mSB.com for ex. IoT HUB

*/
int mSB_Serialize_ping( unsigned char* buffer, unsigned int bufferLen, struct Msb *msb )
{
    int len = 0;

    // Create json formated ping response to msb.com
    len = snprintf(buffer, bufferLen, "pingResponse::{\"nodeName\": \"%s\",\"organizationId\": \"%s\",\"connectionid\": \"%s\",\"status\": \"online\"}",
            msb->name, msb->organizationId, msb->connectionId);

    if( len == bufferLen )
    {
        // Beffer to small
        return -1;
    }

    return 0;    
}

/*
    Function to Deserialize JSON ping response from mSB.com.
    Ping response is sent from mSB.com, connectionID is used when ping messege
    is sent back to mSB.com.
    
    buffer:     message read from network
    bufferLen:  Length of buffer read from network
    msb:        Msb struct holding information from mSB.com for ex. IoT HUB

*/
int mSB_Deserialize_pingResponse( unsigned char* buffer, unsigned int bufferLen, struct Msb *msb )
{
    // ----------JSON------------
    int numTokens = 0;
    
    jsmn_parser jsonParser;
    jsmntok_t jsonTokens[NUMBER_OF_JSON_TOKENS]; // Create memmory space for JSON tokens
    jsmn_init(&jsonParser);

    // Get start and length of json from sign in response
    unsigned char* jsonStart = memchr( buffer, '{', bufferLen );
    int jsonLen = bufferLen - (jsonStart - buffer);

    numTokens = jsmn_parse(&jsonParser, jsonStart, jsonLen, jsonTokens, sizeof(jsonTokens) / sizeof(jsonTokens[0]));

    if( numTokens < 0 )
    {
        // Error parsing json, bad ping response
        return -1;
    }
    else
    {
         if( jsonGetStringValue( jsonStart, numTokens, jsonTokens, "connectionid", msb->connectionId) >= 0)
         {
            return 0;
         }
         else
         {
            // Error parsing connection ID
            msb->connectionId = NULL;
            return -2;
         }
    }
  
 
}

/*
    Function to deserialize reboot message from mSB.com
    No payload in reboot, only trigger soft reset.

    buffer:     message be parsed from network
    bufferLen:  Length of buffer to parse
*/
int mSB_Deserialize_reboot( unsigned char * buffer, int bufferLen )
{
    // No payload
    return 0;
}

/*
    Function to deserialize reset message from mSB.com
    Not implemented in mSB.com

    buffer:     message be parsed from network
    bufferLen:  Length of buffer to parse
*/
int mSB_Deserialize_restart( unsigned char * buffer, int bufferLen )
{
    // Not implemented in mSB.com
    return -1;
}

/*
    Function to deserialize JSON error message from mSB.com
    Typically errors generated when signIn fail.

    buffer:     message be parsed from network
    bufferLen:  Length of buffer to parse
*/
int mSB_Deserialize_errorMessage( unsigned char * buffer, int bufferLen )
{
    // TO DO Print error code to debug interface
    snprintf( stdout, bufferLen, "%s\r\n", buffer );

}

/*
    Function to serialize JSON heartbeat to mSB.com.
    Heartbeat is used to check connection to mSB.com,
    mSB.com shall response with heartbeatResponse.

    buffer:     message to write to network
    bufferLen:  Length of buffer write to network
*/
int mSB_Serialzie_heartbeat( unsigned char * buffer, int bufferLen )
{
    int len = 0;

    // Create and send heartbeat message to msb.com
    len = snprintf(buffer, bufferLen, "heartBeat::{}");
    
    if( len == bufferLen )
    {
        // Error buffer to small
        return -1;
    }

    return 0;
 
}

/*
    Function to deserialize heartbeat response
    
    buffer:     message be parsed from network
    bufferLen:  Length of buffer to parse
*/
int mSB_Deserialize_heartBeatResponse( unsigned char * buffer, int bufferLen )
{
    // No payload yet
    return 0;
}

/*
    Function to deserialize connectionSucess
    ConnectionSucess is sent from mSB.com after
    wss sucessfully connect.

    buffer:     message be parsed from network
    bufferLen:  Length of buffer to parse
*/
int mSB_Deserialize_connectionSucess( unsigned char * buffer, int bufferLen )
{
    // No payload yet
    return 0;
}


int mSB_Serialize_logMessage( unsigned char * buffer, int bufferLen, struct Msb *msb, unsigned char* message )
{
    int len = 0;

    // Create json formated ping response to msb.com
    len = snprintf(buffer, bufferLen, "logMessage::{\"organizationId\": \"%s\",\"nodeName\": \"%s\",\"message\": \"%s\"}",
            msb->organizationId, msb->nodeName, message);

    if( len == bufferLen )
    {
        // Beffer to small, message to long
        return -1;
    }

    return 0;  
}



// Function to check if token is equal to string s
static int jsoneq(const char *json, jsmntok_t *tok, const char *s)
{
    if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
        strncmp(json + tok->start, s, tok->end - tok->start) == 0)
    {
        return 0;
    }
    return -1;
}

// Function to get specific token string value from jsmn parsed tokens
static int jsonGetStringValue(const char *jsonString, const int numTokens, jsmntok_t *t, const char *token, char *value)
{
    if (numTokens < 0)
    {
        printf("[JSON GET TOKEN] Failed to parse JSON: %d\r\n", numTokens);
        return -1;
    }

    // Assume the top-level element is an object
    if (numTokens < 1 || t[0].type != JSMN_OBJECT)
    {
        printf("[JSON GET TOKEN] Object expected\r\n");
        return -2;
    }

    // Step throw json
    for (int i = 1; i < numTokens; i++)
    {
        // Check if tokem match, if match copy value and return
        if (jsoneq(jsonString, &t[i], token) == 0)
        {
            if( t[1]->type == JSMN_STRING )
            {
                // Return string value
                strncpy(value, jsonString + t[i + 1].start, t[i + 1].end - t[i + 1].start);
                value[t[i + 1].end - t[i + 1].start] = '\0'; // Add NULL termination to string
                return 0;
            }
            else{
                // Error expected string but token is not
                printf("[JSON GET TOKEN] String expected\r\n");
                return -3;
            }
        }
        else
        {
            //printf("Unexpected key: %.*s\n", t[i].end-t[i].start,
            //jsonString + t[i].start);
        }
    }
    // TO DO, handle no match
    return -4;
}

//************* TO DO PARSE INT, BOOL, FLOAT **************************


// Function to get specific token integer value from jsmn parsed tokens
static int jsonGetIntegerValue(const char *jsonString, const int numTokens, jsmntok_t *t, const char *token, int *value)
{
    char strValue[10] = {0}; // String for holding json value

    if (numTokens < 0)
    {
        printf("[JSON GET INT TOKEN] Failed to parse JSON: %d\r\n", numTokens);
        return -1;
    }

    // Assume the top-level element is an object
    if (numTokens < 1 || t[0].type != JSMN_OBJECT)
    {
        printf("[JSON GET INT TOKEN] Object expected\r\n");
        return -2;
    }

    // Step throw json
    for (int i = 1; i < numTokens; i++)
    {
        // Check if tokem match, if match copy value and return
        if (jsoneq(jsonString, &t[i], token) == 0)
        {
            if( t[1]->type == JSMN_PRIMITIVE )
            {
                // Return string value
                strncpy(strValue, jsonString + t[i + 1].start, t[i + 1].end - t[i + 1].start);
                value[t[i + 1].end - t[i + 1].start] = '\0'; // Add NULL termination to string
                return 0;
            }
            else{
                // Error expected integer but token is not
                printf("[JSON GET INT TOKEN] Primitive expected\r\n");
                return -3;
            }
        }
        else
        {
            //printf("Unexpected key: %.*s\n", t[i].end-t[i].start,
            //jsonString + t[i].start);
        }
    }
    // TO DO, handle no match
    return -4;
}
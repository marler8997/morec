#ifndef SID_H
#define SID_H

//
// Sid Use Cases
//
// -----------------------------------------------------------------------------
// Sid Generation
// -----------------------------------------------------------------------------
// 1. Generate Sid -> hashUint32
//      - Use uint32_t[5] allocated on the stack
// 2. Convert Sid hashUint32 to hashBytes form (overriding the hashUint32 form)
//    so that it can be encoded to the hashBase64 and stored in the database
//      - This will require a htonl operation on each uint32, and then a cast to
//        and unsigned char *
// 3. Store the Sid in the database using hashBytes with mysql_real_query
// 4. Encode the sid to the hashBase64 form
//      - Use char[27] allocated on the stack to store the encoded string
// 5. Insert the encoded form into the cookie that is in the Set-Cookie Http
//    header.
//
// -----------------------------------------------------------------------------
// Sid Cookie Retrieval
// -----------------------------------------------------------------------------
// 1. Retrieve Sid from cookie -> hashBase64
// 2. Convert to hashBytes
//      - Use unsigned char[20] allocated on the stack
// 3. Use hashBytes2Hex to insert the hash into the buffer holsing the sql query
//    string
// 4. Compare hashBytes with the bytes received from the database query


int generateSid(uint32_t *hash);

#endif

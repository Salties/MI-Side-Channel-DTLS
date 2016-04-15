static const uint8_t ConstantValue[MAX_PAYLOAD_LEN] = {0};

static int ApplicationCode(/*...DTLS arguments...*/) 
{  
  //Replies with a constant value on Requests.
  dtls_write(ctx, session, ConstantValue, RESPONSE_LENGTH);
  
  return 0;
}

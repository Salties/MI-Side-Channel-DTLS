uint8_t datablock[n][AES_BLOCK_LEN];
/* ... */
AES_128.set_key(Aes128Key);
start = RTIMER_NOW();
for (i = 0; i < n; i++) 
{ 
    AES_128.encrypt(datablock[i]); //Used for both plaintext and ciphertext.
}
end = RTIMER_NOW();
AesTime = end - start;

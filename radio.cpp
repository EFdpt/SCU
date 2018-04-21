#include "model.h"
#include "radio.h"

#include <AES.h>

AES aes;

int encrypt_model(String model, byte* cipher, size_t cipher_max_length) {
	
	size_t model_length = model.length();
	int cipher_length = model_length + (N_BLOCK - ((model_length - 1) & 0x10)); //  % 16));
	if (cipher_length > cipher_max_length) // cipher byte array not too big to store encrypted data
		return 0;

	aes.iv_inc();
	byte iv[N_BLOCK];

	aes.set_IV(RADIO_IV);
	aes.get_IV(iv);
	aes.do_aes_encrypt(model.c_str(), model_length, cipher, RADIO_KEY, RADIO_KEY_BITS, iv);

	return cipher_length;
}
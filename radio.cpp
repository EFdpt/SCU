#include <ArduinoJson.h>

#include "model.h"
#include "radio.h"

#define JSON_BUFFER_SIZE      JSON_OBJECT_SIZE(2) + 3*JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(11)
#define CIPHER_MAX_LENGTH     1024

byte plain[CIPHER_MAX_LENGTH];
byte cipher[CIPHER_MAX_LENGTH];
AES_ctx ctx;

int encrypt_model(byte* model, size_t model_length, byte* cipher, size_t cipher_max_length) {
	
	int cipher_length = model_length + (N_BLOCK - ((model_length - 1) & 0x10)); //  % 16));
	if (cipher_length > cipher_max_length) // cipher byte array not too big to store encrypted data
		return 0;

  AES_init_ctx_iv(&ctx, (const uint8_t*) key, (const uint8_t*) IV);
	AES_CTR_xcrypt_buffer(&ctx, (uint8_t*) model, cipher_length);

	return cipher_length;
}

void radio_send_model() {
  StaticJsonBuffer<JSON_BUFFER_SIZE>  jsonBuffer;
  String                              model = "";
  size_t                              model_len;
  size_t                              cipher_len;

  JsonObject&   root = jsonBuffer.createObject();

  JsonObject& pedals = root.createNestedObject("pedals");
  JsonObject& suspensions = root.createNestedObject("suspensions");
  JsonObject& wheels = root.createNestedObject("wheels");
  JsonObject& accelerometers = root.createNestedObject("accelerometers");

  pedals["tps1"] = tps1_percentage;
  pedals["tps2"] = tps1_percentage;
  pedals["brake"] = brake_percentage;
  /*
  pedals["tps1_min"] = tps1_low;
  pedals["tps1_max"] = tps1_max;
  pedals["tps2_min"] = tps2_low;
  pedals["tps2_max"] = tps2_max;
  pedals["brake_min"] = brake_low;
  pedals["brake_max"] = brake_max; */
  pedals["plaus1"] = plaus1;
  pedals["plaus2"] = plaus2;
  suspensions["front_sx"] = fr_sx_susp;
  suspensions["front_dx"] = fr_dx_susp;
  suspensions["retro_sx"] = rt_sx_susp;
  suspensions["retro_dx"] = rt_dx_susp;
  wheels["front_sx"] = fr_sx_rpm;
  wheels["front_dx"] = fr_dx_rpm;
  wheels["retro_sx"] = rt_sx_rpm;
  wheels["retro_dx"] = rt_dx_susp;
  accelerometers["acc_x"] = acc_x_value;
  accelerometers["acc_y"] = acc_y_value;

  root.printTo(model);
  model_len = model.length();
  strncpy((char*) plain, model.c_str(), model_len);
  
  if (!(cipher_len = encrypt_model(plain, model_len, cipher, CIPHER_MAX_LENGTH)))
    return;   // error: string too long

  


  //SPI_send_string(log);
}
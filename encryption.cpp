#include "encryption.h"
#include <inttypes.h>

/***************************************************************************
 * 2. DEFINES                                                              *
 ***************************************************************************/
// concatenates 4 � 8-bit words (= 1 byte) to one 32-bit word
#define CONCAT_4_BYTES( w32, w8, w8_i)           \
{                                                \
(w32) = ( (uint32_t) (w8)[(w8_i)    ] << 24 ) |  \
( (uint32_t) (w8)[(w8_i) + 1] << 16 ) |  \
( (uint32_t) (w8)[(w8_i) + 2] <<  8 ) |  \
( (uint32_t) (w8)[(w8_i) + 3]       );   \
}


// splits a 32-bit word into 4 � 8-bit words (= 1 byte)
#define SPLIT_INTO_4_BYTES( w32, w8, w8_i)       \
{                                                \
(w8)[(w8_i)    ] = (uint8_t) ( (w32) >> 24 );    \
(w8)[(w8_i) + 1] = (uint8_t) ( (w32) >> 16 );    \
(w8)[(w8_i) + 2] = (uint8_t) ( (w32) >>  8 );    \
(w8)[(w8_i) + 3] = (uint8_t) ( (w32)       );    \
}


// get x-th byte of 32 bit word
#define BYTE_0(w32) ( (uint8_t) (w32 >> 24) )
#define BYTE_1(w32) ( (uint8_t) (w32 >> 16) )
#define BYTE_2(w32) ( (uint8_t) (w32 >>  8) )
#define BYTE_3(w32) ( (uint8_t) (w32      ) )



// compute S-Box substitutions during execution
#define FORWARD_SUB_BYTE(input) forward_s_box_compute((input))


// compute S-Box substitution during execution
#define INVERSE_SUB_BYTE(input) inverse_s_box_compute((input))


/***************************************************************************
 * 3. DEFINITIONS                                                          *
 ***************************************************************************/

/***************************************************************************
 * 4. CONSTANTS                                                            *
 ***************************************************************************/

// define the GF2^8 irreducible field polynomial 0x11B = x^8 + x^4 + x^3 + x + 1
#define GF2_8_FIELD_POLYNOMIAL 0x1B

/***************************************************************************
 * 6. IMPLEMENTATION OF FUNCTIONS                                          *
 ***************************************************************************/


// round constants = [2^i in GF(2^8)]
static const uint8_t rcon[10] =
{
	0x01, 0x02, 0x04, 0x08, 0x10,
	0x20, 0x40, 0x80, 0x1B, 0x36
};




void mem_copy(uint8_t* dest, const uint8_t* source)
{
	// declarations
	int i;
	
	// copy 16 bytes
	for ( i = 0; i < 16; i++ )
		dest[i] = source[i];
}




static uint8_t GF2_8_field_mult_by_2( uint8_t a )
{
	// declarations
	uint8_t r[2];
	
	// left shift = mult by 0x02
	r[0] = ( a << 1 );
	
	// if MSB  was 1 then reduce by field polynomial
	r[1] = r[0] ^ GF2_8_FIELD_POLYNOMIAL;
	
	// this is for SPA resistance
	r[0] = r[(a & 0x80) != 0x0];
	
	// return result
	return r[0];
}



static uint32_t forward_mix_col(uint32_t state_in)
{
	// declarations
	uint32_t state_out;
	uint8_t t, v;
	
	// t = a[0] + a[1] + a[2] + a[3]
	t = BYTE_0( state_in ) ^ BYTE_1( state_in ) ^ BYTE_2( state_in ) ^ BYTE_3( state_in );
	
	// v = a[0] + a[1]
	// v = x*v
	// r[0] = a[0] + v + t
	v = BYTE_0 ( state_in ) ^ BYTE_1( state_in );
	v = GF2_8_field_mult_by_2( v );
	state_out = ( BYTE_0( state_in ) ^ v ^ t) << 24;
	
	// v = a[1] + a[2]
	// v = x*v
	// r[1] = a[1] + v + t
	v = BYTE_1( state_in ) ^ BYTE_2( state_in );
	v = GF2_8_field_mult_by_2( v );
	state_out ^= ( BYTE_1( state_in ) ^ v ^ t) << 16;
	
	// v = a[2] + a[3]
	// v = x*v
	// r[2] = a[2] + v + t
	v = BYTE_2( state_in ) ^ BYTE_3( state_in );
	v = GF2_8_field_mult_by_2( v );
	state_out ^= ( BYTE_2( state_in ) ^ v ^ t) << 8;
	
	// v = a[3] + a[0]
	// v = x*v
	// r[3] = a[3] + v + t
	v = BYTE_3( state_in ) ^ BYTE_0( state_in );
	v = GF2_8_field_mult_by_2( v );
	state_out ^= ( BYTE_3( state_in ) ^ v ^ t);
	
	// return result
	return state_out;
}






static uint32_t inverse_mix_col(uint32_t state_in)
{
	// declarations
	uint32_t state_out;
	uint8_t u, v;
	
	// u = x*(x*(a[0] + a[2]))
	u = GF2_8_field_mult_by_2( BYTE_0(state_in) ^ BYTE_2(state_in) );
	u = GF2_8_field_mult_by_2( u );
	
	// v = v*(v*(a[1] + a[3]))
	v = GF2_8_field_mult_by_2( BYTE_1(state_in) ^ BYTE_3(state_in) );
	v = GF2_8_field_mult_by_2( v );
	
	// a[0] = a[0] + u
	// a[1] = a[1] + v
	// a[2] = a[2] + u
	// a[3] = a[3] + v
	state_out = ( ( (BYTE_0(state_in) ^ u) << 24) ^
				 ( (BYTE_1(state_in) ^ v) << 16) ^
				 ( (BYTE_2(state_in) ^ u) <<  8) ^
				 ( (BYTE_3(state_in) ^ v)      ) );
	
	// MixCol( a )
	state_out = forward_mix_col( state_out );
	
	// return result
	return state_out;
}







static uint8_t highbit( const uint8_t x )
{
	// declarations
	uint8_t r = (uint8_t)((x >> 1) | (x >> 2));
	
	r |= (r >> 2);
	r |= (r >> 4);
	
	// return result
	return (r + 1) >> 1;
}





static uint8_t inverse_element( uint8_t input )
{
	// declarations
	uint8_t p1 = input;
	uint8_t p2 = GF2_8_FIELD_POLYNOMIAL;
	uint8_t n1 = highbit(input);
	uint8_t n2 = 0x80;
	uint8_t v1 = 1;
	uint8_t v2 = 0;
	
	// return 0x00 if input = 0x00
	// return 0x01 if input = 0x01
	if(input < 2) return input;
	
	// compute the inverse by Euclid
	for(;;)
	{
		// if n1 == 0, Euclid is finished
		if(!n1)
			return v1;
		
		// iterate Euclid
		while(n2 >= n1)
		{
			n2 /= n1;
			p2 ^= p1 * n2;
			v2 ^= v1 * n2;
			n2 = highbit(p2);
		}
		
		// if n2 == 0, Euclid is finished
		if(!n2)
			return v2;
		
		// iterate Euclid
		while(n1 >= n2)
		{
			n1 /= n2;
			p1 ^= p2 * n1;
			v1 ^= v2 * n1;
			n1 = highbit(p1);
		}
	}
}



uint8_t forward_s_box_compute( uint8_t input )
{
	// declarations
	uint32_t w32;
	
	// compute inverse element
	w32 = (uint32_t) ( inverse_element(input) );
	
	// apply linear transformation
	w32 ^= (w32<<1) ^ (w32<<2) ^ (w32<<3) ^ (w32<<4);
	
	// reduce result and apply affine transformation
	return ( 0x63 ^ (uint8_t)(w32 ^ (w32>>8) ) );
}



uint8_t inverse_s_box_compute( uint8_t input )
{
	// declarations
	uint32_t w32;
	
	// convert input to 32-bit word
	w32 = (uint32_t)(input);
	
	// apply linear transformation
	w32 = (w32<<1) ^ (w32<<3) ^ (w32<<6);
	
	// reduce result, apply affine transformation, and inverse result
	return inverse_element( 0x05 ^ (uint8_t)( w32^(w32>>8) ) );
}




static int aes_set_key( /*@out@*/ aes_context *ctx,
					   const uint8_t       *key,
					   const uint8_t       *IV )
{
	// declarations
	int i;
	uint32_t *enc_round_key;
	
	
	
	
	// get pointer to encryption round keys
	enc_round_key = ctx->enc_round_keys;
	
	// enc_round_key[0..3] is the original key
	for ( i = 0; i < 4; i++ )
	{
		CONCAT_4_BYTES( enc_round_key[i], key, i * 4 );
	}
	
	// derive enc_round_key[4..43] from enc_round_key[0..3]
	for ( i = 0; i < 10; i++ )
	{
		// enc_round_key[i mod 4 == 0] = .. where i = this i * 4 (means i from FIPS-197 Fig.11)
		enc_round_key[4]  = (  enc_round_key[0]                                                         ) ^ /* 32-bit w[i-4]                                                        */
		( (FORWARD_SUB_BYTE( (int)( BYTE_1( enc_round_key[3] ) ) ) ^ rcon[i]) << 24 ) ^ /* 32-bit { S-box( enc_round_key[i-1]-a1 ),                             */
		(  FORWARD_SUB_BYTE( (int)( BYTE_2( enc_round_key[3] ) ) )            << 16 ) ^ /*          S-box( enc_round_key[i-1]-a2 ),                             */
		(  FORWARD_SUB_BYTE( (int)( BYTE_3( enc_round_key[3] ) ) )            <<  8 ) ^ /*          S-box( enc_round_key[i-1]-a3 ),                             */
		(  FORWARD_SUB_BYTE( (int)( BYTE_0( enc_round_key[3] ) ) )                  );  /*          S-box( enc_round_key[i-1]-a0 ) } = SubWord(RotWord(w[i-1])) */
		
		// enc_round_key[i mod 4 != 0] = w[i-4] XOR w[i-1], where i = this i * 4 (means i from FIPS-197 Fig.11)
		enc_round_key[5]  = enc_round_key[1] ^ enc_round_key[4];
		enc_round_key[6]  = enc_round_key[2] ^ enc_round_key[5];
		enc_round_key[7]  = enc_round_key[3] ^ enc_round_key[6];
		
		// next 4 32-bit words
		enc_round_key += 4;
	}
	// break;
	
	// set initialization vector
    if ( IV != 0 )
	{
		mem_copy( ctx->IV, IV );
	}
	
	// successful
	return  0;
}




static int aes_encrypt_16_byte_block( const aes_context *ctx,
									 const uint8_t       *plain,
/*@out@*/ uint8_t       *cipher )
{
	// declarations
	uint32_t *round_key;          // pointer to round key
	uint32_t cx0, cx1, cx2, cx3;  // state array columns, cx for input
	uint32_t cy0, cy1, cy2, cy3;  // state array columns, cy for output
	uint8_t i;                    // counter for encryption loop
	
	
	
	// get encryption round keys
	round_key = (uint32_t *) ctx->enc_round_keys;
	
	// read 16 plain text bytes into four 32-bit words (FIPS-197: state = in )
	CONCAT_4_BYTES( cx0, plain,  0 );
	CONCAT_4_BYTES( cx1, plain,  4 );
	CONCAT_4_BYTES( cx2, plain,  8 );
	CONCAT_4_BYTES( cx3, plain,  12 );
	
	// XOR it with encryption round_key[0..3] (FIPS-197: AddRoundKey(state))
	cx0 ^= round_key[0];
	cx1 ^= round_key[1];
	cx2 ^= round_key[2];
	cx3 ^= round_key[3];
	
	// do encryption rounds 1..9
	// forward S-box, ShiftRows() via input structure, MixCol, XOR round_key
	round_key+=4;
	i = 0;
	while ( i < 9 )
	{
		cy0 = round_key[0] ^ forward_mix_col( ( ( (uint32_t) (FORWARD_SUB_BYTE( (int) BYTE_0( cx0 ) ) ) ) << 24 ) ^
											 ( ( (uint32_t) (FORWARD_SUB_BYTE( (int) BYTE_1( cx1 ) ) ) ) << 16 ) ^
											 ( ( (uint32_t) (FORWARD_SUB_BYTE( (int) BYTE_2( cx2 ) ) ) ) <<  8 ) ^
											 ( ( (uint32_t) (FORWARD_SUB_BYTE( (int) BYTE_3( cx3 ) ) ) )       ) );
		
		cy1 = round_key[1] ^ forward_mix_col( ( ( (uint32_t) (FORWARD_SUB_BYTE( (int) BYTE_0( cx1 ) ) ) ) << 24 ) ^
											 ( ( (uint32_t) (FORWARD_SUB_BYTE( (int) BYTE_1( cx2 ) ) ) ) << 16 ) ^
											 ( ( (uint32_t) (FORWARD_SUB_BYTE( (int) BYTE_2( cx3 ) ) ) ) <<  8 ) ^
											 ( ( (uint32_t) (FORWARD_SUB_BYTE( (int) BYTE_3( cx0 ) ) ) )       ) );
		
		cy2 = round_key[2] ^ forward_mix_col( ( ( (uint32_t) (FORWARD_SUB_BYTE( (int) BYTE_0( cx2 ) ) ) ) << 24 ) ^
											 ( ( (uint32_t) (FORWARD_SUB_BYTE( (int) BYTE_1( cx3 ) ) ) ) << 16 ) ^
											 ( ( (uint32_t) (FORWARD_SUB_BYTE( (int) BYTE_2( cx0 ) ) ) ) <<  8 ) ^
											 ( ( (uint32_t) (FORWARD_SUB_BYTE( (int) BYTE_3( cx1 ) ) ) )       ) );
		
		cy3 = round_key[3] ^ forward_mix_col( ( ( (uint32_t) (FORWARD_SUB_BYTE( (int) BYTE_0( cx3 ) ) ) ) << 24 ) ^
											 ( ( (uint32_t) (FORWARD_SUB_BYTE( (int) BYTE_1( cx0 ) ) ) ) << 16 ) ^
											 ( ( (uint32_t) (FORWARD_SUB_BYTE( (int) BYTE_2( cx1 ) ) ) ) <<  8 ) ^
											 ( ( (uint32_t) (FORWARD_SUB_BYTE( (int) BYTE_3( cx2 ) ) ) )       ) );
		
		round_key += 4;
		i++;
		
		// copy cy --> cx for input for next round
		cx0 = cy0;
		cx1 = cy1;
		cx2 = cy2;
		cx3 = cy3;
	}
	
	// final forward S-box round inluding ShiftRows() via input structure
	cy0 = round_key[0] ^ ( ( ( (uint32_t) (FORWARD_SUB_BYTE( (int) BYTE_0( cx0 ) ) ) ) << 24 ) ^
						  ( ( (uint32_t) (FORWARD_SUB_BYTE( (int) BYTE_1( cx1 ) ) ) ) << 16 ) ^
						  ( ( (uint32_t) (FORWARD_SUB_BYTE( (int) BYTE_2( cx2 ) ) ) ) <<  8 ) ^
						  ( ( (uint32_t) (FORWARD_SUB_BYTE( (int) BYTE_3( cx3 ) ) ) )       ) );
	
	cy1 = round_key[1] ^ ( ( ( (uint32_t) (FORWARD_SUB_BYTE( (int) BYTE_0( cx1 ) ) ) ) << 24 ) ^
						  ( ( (uint32_t) (FORWARD_SUB_BYTE( (int) BYTE_1( cx2 ) ) ) ) << 16 ) ^
						  ( ( (uint32_t) (FORWARD_SUB_BYTE( (int) BYTE_2( cx3 ) ) ) ) <<  8 ) ^
						  ( ( (uint32_t) (FORWARD_SUB_BYTE( (int) BYTE_3( cx0 ) ) ) )       ) );
	
	cy2 = round_key[2] ^ ( ( ( (uint32_t) (FORWARD_SUB_BYTE( (int) BYTE_0( cx2 ) ) ) ) << 24 ) ^
						  ( ( (uint32_t) (FORWARD_SUB_BYTE( (int) BYTE_1( cx3 ) ) ) ) << 16 ) ^
						  ( ( (uint32_t) (FORWARD_SUB_BYTE( (int) BYTE_2( cx0 ) ) ) ) <<  8 ) ^
						  ( ( (uint32_t) (FORWARD_SUB_BYTE( (int) BYTE_3( cx1 ) ) ) )       ) );
	
	cy3 = round_key[3] ^ ( ( ( (uint32_t) (FORWARD_SUB_BYTE( (int) BYTE_0( cx3 ) ) ) ) << 24 ) ^
						  ( ( (uint32_t) (FORWARD_SUB_BYTE( (int) BYTE_1( cx0 ) ) ) ) << 16 ) ^
						  ( ( (uint32_t) (FORWARD_SUB_BYTE( (int) BYTE_2( cx1 ) ) ) ) <<  8 ) ^
						  ( ( (uint32_t) (FORWARD_SUB_BYTE( (int) BYTE_3( cx2 ) ) ) )       ) );
	
	// write result into cipher text byte array (FIPS 197: out = state )
	SPLIT_INTO_4_BYTES( cy0, cipher,  0 );
	SPLIT_INTO_4_BYTES( cy1, cipher,  4 );
	SPLIT_INTO_4_BYTES( cy2, cipher,  8 );
	SPLIT_INTO_4_BYTES( cy3, cipher, 12 );
	
	// successful
	return 0;
}




static int aes_decrypt_16_byte_block( const aes_context *ctx,
									 const uint8_t       *cipher,
/*@out@*/ uint8_t       *plain )
{
	// declarations
	uint32_t *round_key;           // pointer to round key
	uint32_t cx0, cx1, cx2, cx3;   // state array columns, cx for input
	uint32_t cy0, cy1, cy2, cy3;   // state array columns, cy for output
	int i;                       // counter for decryption loop
	
	
	
	// get decryption round keys --> changed: get encryption round keys
	round_key = (uint32_t *) ctx->enc_round_keys;
	round_key += 40;
	
	// read 16 cipher text bytes into four 32-bit words (FIPS-197: state = in )
	CONCAT_4_BYTES( cx0, cipher,  0 );
	CONCAT_4_BYTES( cx1, cipher,  4 );
	CONCAT_4_BYTES( cx2, cipher,  8 );
	CONCAT_4_BYTES( cx3, cipher, 12 );
	
	// XOR it with enc_round_key[43..40]
	cx0 ^= round_key[0];
	cx1 ^= round_key[1];
	cx2 ^= round_key[2];
	cx3 ^= round_key[3];
	
	// do decryption rounds 9..1
	// inverse S-box, inverse ShiftRows() via input structure, inverseMixCol, XOR round_key
	i = 0;
	round_key -= 4;
	while ( i < 9 )
	{
		
		
		cy0 = inverse_mix_col( round_key[0] ^ ( ( (uint32_t) INVERSE_SUB_BYTE( (int)( BYTE_0( cx0 ) ) ) << 24 ) ^
											   ( (uint32_t) INVERSE_SUB_BYTE( (int)( BYTE_1( cx3 ) ) ) << 16 ) ^
											   ( (uint32_t) INVERSE_SUB_BYTE( (int)( BYTE_2( cx2 ) ) ) <<  8 ) ^
											   ( (uint32_t) INVERSE_SUB_BYTE( (int)( BYTE_3( cx1 ) ) )       ) ) );
		
		cy1 = inverse_mix_col( round_key[1] ^ ( ( (uint32_t) INVERSE_SUB_BYTE( (int)( BYTE_0( cx1 ) ) ) << 24 ) ^
											   ( (uint32_t) INVERSE_SUB_BYTE( (int)( BYTE_1( cx0 ) ) ) << 16 ) ^
											   ( (uint32_t) INVERSE_SUB_BYTE( (int)( BYTE_2( cx3 ) ) ) <<  8 ) ^
											   ( (uint32_t) INVERSE_SUB_BYTE( (int)( BYTE_3( cx2 ) ) )       ) ) );
		
		cy2 = inverse_mix_col( round_key[2] ^ ( ( (uint32_t) INVERSE_SUB_BYTE( (int)( BYTE_0 ( cx2 ) ) ) << 24 ) ^
											   ( (uint32_t) INVERSE_SUB_BYTE( (int)( BYTE_1 ( cx1 ) ) ) << 16 ) ^
											   ( (uint32_t) INVERSE_SUB_BYTE( (int)( BYTE_2 ( cx0 ) ) ) <<  8 ) ^
											   ( (uint32_t) INVERSE_SUB_BYTE( (int)( BYTE_3 ( cx3 ) ) )       ) ) );
		
		cy3 = inverse_mix_col( round_key[3] ^ ( ( (uint32_t) INVERSE_SUB_BYTE( (int)( BYTE_0 ( cx3 ) ) ) << 24 ) ^
											   ( (uint32_t) INVERSE_SUB_BYTE( (int)( BYTE_1 ( cx2 ) ) ) << 16 ) ^
											   ( (uint32_t) INVERSE_SUB_BYTE( (int)( BYTE_2 ( cx1 ) ) ) <<  8 ) ^
											   ( (uint32_t) INVERSE_SUB_BYTE( (int)( BYTE_3 ( cx0 ) ) )       ) ) );
		
		i++;
		round_key -= 4;
		
		// copy cy --> cx for input for next round
		cx0 = cy0;
		cx1 = cy1;
		cx2 = cy2;
		cx3 = cy3;
	}
	
	// final round:
	cy0 = round_key[0] ^ ( ( (uint32_t) INVERSE_SUB_BYTE( (int)( BYTE_0( cx0 ) ) ) << 24 ) ^
						  ( (uint32_t) INVERSE_SUB_BYTE( (int)( BYTE_1( cx3 ) ) ) << 16 ) ^
						  ( (uint32_t) INVERSE_SUB_BYTE( (int)( BYTE_2( cx2 ) ) ) <<  8 ) ^
						  ( (uint32_t) INVERSE_SUB_BYTE( (int)( BYTE_3( cx1 ) ) )       ) );
	
	cy1 = round_key[1] ^ ( ( (uint32_t) INVERSE_SUB_BYTE( (int)( BYTE_0( cx1 ) ) ) << 24 ) ^
						  ( (uint32_t) INVERSE_SUB_BYTE( (int)( BYTE_1( cx0 ) ) ) << 16 ) ^
						  ( (uint32_t) INVERSE_SUB_BYTE( (int)( BYTE_2( cx3 ) ) ) <<  8 ) ^
						  ( (uint32_t) INVERSE_SUB_BYTE( (int)( BYTE_3( cx2 ) ) )       ) );
	
	cy2 = round_key[2] ^ ( ( (uint32_t) INVERSE_SUB_BYTE( (int)( BYTE_0( cx2 ) ) ) << 24 ) ^
						  ( (uint32_t) INVERSE_SUB_BYTE( (int)( BYTE_1( cx1 ) ) ) << 16 ) ^
						  ( (uint32_t) INVERSE_SUB_BYTE( (int)( BYTE_2( cx0 ) ) ) <<  8 ) ^
						  ( (uint32_t) INVERSE_SUB_BYTE( (int)( BYTE_3( cx3 ) ) )       ) );
	
	cy3 = round_key[3] ^ ( ( (uint32_t) INVERSE_SUB_BYTE( (int)( BYTE_0( cx3 ) ) ) << 24 ) ^
						  ( (uint32_t) INVERSE_SUB_BYTE( (int)( BYTE_1( cx2 ) ) ) << 16 ) ^
						  ( (uint32_t) INVERSE_SUB_BYTE( (int)( BYTE_2( cx1 ) ) ) <<  8 ) ^
						  ( (uint32_t) INVERSE_SUB_BYTE( (int)( BYTE_3( cx0 ) ) )       ) );
	
	// write result into plain text byte array
	SPLIT_INTO_4_BYTES( cy0, plain,  0 );
	SPLIT_INTO_4_BYTES( cy1, plain,  4 );
	SPLIT_INTO_4_BYTES( cy2, plain,  8 );
	SPLIT_INTO_4_BYTES( cy3, plain, 12 );
	
	// successful
	return 0;
	
}




extern uint8_t aes_encrypt( const uint8_t   aes_mode,
						   const uint8_t  *aes_key,
						   const uint32_t  aes_key_length,
						   const uint8_t  *IV,
						   const uint32_t  IV_length,
						   uint8_t  *plain_data,
/*@out@*/ uint8_t  *cipher_data,
						   const uint32_t  data_length )
{
	// declarations
	aes_context aes_ctx; // aes context
	uint8_t aes_buf[16];   // temporary buffer of one block
	uint32_t num_blocks;   // number of blocks to encrypt
	uint8_t *p_plain;      // pointer to plain text block
	uint8_t *p_cipher;     // pointer to cipher text block
	uint32_t i;
	int ret;
	
	
	
	// init encryption keys
	ret = aes_set_key( &aes_ctx, aes_key, IV );
	assert( ret == 0 );
	
	// get number of blocks and padding
	num_blocks = (uint32_t) ( data_length / 16 );
	
	// get pointers
	p_plain  = plain_data;
	p_cipher = cipher_data;
	
	// --- Electronic Codebook Mode (ECB) -------------------------------------
	if ( aes_mode == C_AES_MODE_ECB )
	{
		// encrypt all 16 byte blocks
		for ( i = 0; i < num_blocks; i++ )
		{
			// fill buffer with plain
			mem_copy( aes_buf, p_plain );
			
			// encrypt buffer
			ret = aes_encrypt_16_byte_block( &aes_ctx, aes_buf, aes_buf );
			assert( ret == 0 );
			
			// write cipher
			mem_copy( p_cipher, aes_buf );
			
			// next block
			p_plain  += 16;
			p_cipher += 16;
		}
	}else
		return E_AES_WRONG_MODE;
	
	
	
	
	
	// successful
	return E_AES_SUCCESS;
	
}



extern uint8_t aes_decrypt( const uint8_t   aes_mode,
						   const uint8_t  *aes_key,
						   const uint32_t  aes_key_length,
						   const uint8_t  *IV,
						   const uint32_t  IV_length,
						/*@out@*/ uint8_t  *plain_data,
						   uint8_t  *cipher_data,
						   const uint32_t  data_length )
{
	// declarations
	aes_context aes_ctx; // aes context
	uint8_t aes_buf1[16];  // buffer for one 16-byte block
	//  uint8_t aes_buf2[16];  // buffer for one 16-byte block
	uint32_t num_blocks;   // number of blocks to decrypt
	uint8_t *p_plain;      // pointer to plain data block
	uint8_t *p_cipher;     // pointer to cipher data block
	uint32_t i;
	int ret;
	
	
	
	
	// init encryption keys
	ret = aes_set_key( &aes_ctx, aes_key, IV );
	assert( ret == 0 );
	
	// get number of blocks and padding
	num_blocks = (uint32_t) ( data_length / 16 );
	
	// get pointers
	p_plain  = plain_data;
	p_cipher = cipher_data;
	
	// --- Electronic Codebook Mode (ECB) -------------------------------------
	
	if ( aes_mode == C_AES_MODE_ECB )
	{
		
		// decrypt 16 byte blocks
		for ( i = 0; i < num_blocks; i++ )
		{
			// fill buffer 1 with cipher
			mem_copy( aes_buf1, p_cipher );
			
			// decrypt buffer 1
			ret = aes_decrypt_16_byte_block( &aes_ctx, aes_buf1, aes_buf1 );
			assert( ret == 0 );
			
			// write buffer 1 to plain
			mem_copy( p_plain, aes_buf1 );
			
			// next block
			p_plain  += 16;
			p_cipher += 16;
		}
		
	}else
		return E_AES_WRONG_MODE;
	
	
	// successful
	return E_AES_SUCCESS;
	
}



unsigned short cal_crc_loop_CCITT_A( short l, unsigned char* p, unsigned short seed,unsigned short cd )
{
	int i, j;
	unsigned short crc = seed;
	
	for ( i = 0; i < l; ++i )
	{
		crc ^= ( p[ i ] << 8 );
		for ( j = 0; j < 8; ++j )
		{
			if ( crc & 0x8000 )
				crc = ( crc << 1 ) ^ cd;
			else
				crc <<= 1;
		}
	}
	return crc;
}

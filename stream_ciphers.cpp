//Juliette Rocco
//November 14th 2019
//Stream Ciphers
//Encrypts and decrypts strings

#include <iostream>
#include <cassert>
#include <cmath>

//function declarations
int main();
char *encode( char *plaintext, unsigned long key );
char *decode( char *ciphertext, unsigned long key );

#ifndef MARMOSET_TESTING
int main(){

	//strings to send into the functions
	char str0[]{"study"};
	char str1[]{"A Elbereth Gilthoniel\nsilivren penna miriel\n""o menel aglar elenath!\nNa-chaered palan-diriel\n""o galadhremmin ennorath,\nFanuilos, le linnathon\n""nef aear, si nef aearon!"};

	/* prints out the first string and sends it to the encode
	 * and then the decode function
	 */
	std::cout << "\"" << str0 << "\"" << std::endl;
	char *ciphertext{ encode( str0, 3408) };
	std::cout << "\"" <<ciphertext << "\"" << std::endl;
	char *plaintext{ decode( ciphertext, 3408)};
	std::cout << "\"" <<plaintext << "\"" << std::endl;


	//deletes the pointers and changes them to nullptr
	delete [] plaintext;
	plaintext = nullptr;
	delete [] ciphertext;
	ciphertext = nullptr;


	/*performs the same steps as above but
	 * for the second string
	 */
	std::cout << "\"" << str1 << "\"" << std::endl;
	ciphertext= encode( str1, 3408);
	std::cout << "\"" <<ciphertext << "\"" << std::endl;
	plaintext= decode( ciphertext, 3408);
	std::cout << "\"" <<plaintext << "\"" << std::endl;

	delete [] plaintext;
	plaintext = nullptr;
	delete [] ciphertext;
	ciphertext = nullptr;

	return 0;
}
#endif


char *encode( char *plaintext, unsigned long key ){
	unsigned char S[256]{};						//state array
	unsigned int i{}, j{}, k{};					//variables to arrange state array
	unsigned int length{}, left{}, counter{};	//length of string, leftover to make divisible by 4, index counter
	bool end{false};							//end of string loop variable




	 /*determine the length of the c string
	 * by counting the characters till the null char
	 */
	 while( end == false){
		 if (plaintext[length] == '\0'){
			 end = true;
		 }
		 else{
			 ++length;
		 }
	 }

	 /*if the length is not divisible by 4
	 * it adds to the length to make it divisible by 4
	 */
	 if ((length % 4) != 0){
		 left= 4-(length % 4);
		 length += left;
	 }
	//populate the S array with numbers 0 to 255
	for (std::size_t l{0}; l < 256; ++l){
		S[l] = l;
	}

	/*randomize the entries in S using the algorithm provided
	* in the instruction document, finds the kth bit by doing bitshifts
	*/
	for (std::size_t m{}; m < 256; ++m){
		k = i % 64;
		j = (j + S[i]  + ((key >> k) & 1UL)) % 256;
		int temp1{S[j]};
		S[j] = S[i];
		S[i] = temp1;
		i = (i + 1 ) % 256;
	}


	//new cipher_text array with the proper length
	unsigned char cipher_text[length]{};

	/*for the original length it populates the cipher_text array
	* with the values that are in plaintext
	*/
	for (unsigned int p = 0; p < length - left; p++) {
		cipher_text[p] = plaintext[p];
	}

	/* for the leftover amount from increasing the length,
	 * populates the cipher_text array with null characters
	 */
	for (unsigned int p = length - left; p < length; p++) {
		cipher_text[p] = '\0';
	}


	/* populates the cipher_text array by XOR with the R value,
	 * formula determined from the instruction document
	 */
	for (std::size_t p{}; p < length; ++p){
		i = ( i + 1) % 256;
		j = ( j + S[i]) % 256;
		unsigned char temp2{S[j]};
		S[j] = S[i];
		S[i] = temp2;
		unsigned int r = (S[i] + S[j]) % 256;
		unsigned int R = S[r];
		cipher_text[p] = (cipher_text[p] ^ R);
	}


	//how many times the loop needs to run
	unsigned int divisible{length / 4};
	//new array to store the ascii_armour text
	char *ascii_armour{ new char [divisible + length + 1]};

	//for every four characters...it creates 5 in ascii armour..
	//this is the ascii armour function
	for (std::size_t q{0}; q < divisible; ++q){
		//sum variable
		unsigned int sum = cipher_text[4*q];

		/*combine the four characters into
		* one integer called sum
		*/
		sum = sum << 8;
		sum += cipher_text[4*q + 1];
		sum = sum << 8;
		sum += cipher_text[4*q + 2];
		sum = sum << 8;
		sum += cipher_text[4*q + 3];

		//counter is the index, increases by 5 each time
		counter +=5;
		unsigned int remainder{};
		for (std::size_t r{0}; r <= 4; ++r){
			/*performs base 85 operations
			* adds the value of ! to make sure the characters
			* are printable, indexes backwards because the order
			* comes out backwards
			*/
			remainder = sum % 85;
			remainder += '!';
			ascii_armour[counter-1] = remainder;
			--counter;
			sum /= 85;
		}
		//adds 5 back to the counter to reset
		counter += 5;

	}

	//adds the end of line string character at the end
	ascii_armour[divisible+length] = '\0';

	return ascii_armour;
}
char *decode( char *ciphertext, unsigned long key ){
		unsigned char S[256]{};					//state array
		unsigned int i{}, j{}, k{};				//variables for state formula
		unsigned int length{}, sum{};			//length of string, sum of the integers
		bool end{false};						//end of the string bool

		//populate the S array with numbers 0 to 255
		for (std::size_t l{0}; l < 256; ++l){
			S[l] = l;
		}

		/*randomize the entries in S using the algorithm provided
		* in the instruction document, finds the kth bit by doing bitshifts
		*/
		for (std::size_t m{}; m < 256; ++m){
			k = i % 64;
			j = (j + S[i]  + ((key >> k) & 1UL)) % 256;
			int temp1{S[j]};
			S[j] = S[i];
			S[i] = temp1;
			i = (i + 1 ) % 256;
		}

		/*determine the length of the c string
		 * by counting the characters till the null char
		 */
		 while( end == false){
			 if (ciphertext[length] == '\0'){
				 end = true;
			 }
			 else{
				 ++length;
			 }
		 }

		 // the proper length of the string
		 unsigned int new_length = (4*(length))/5;


		 unsigned int counter{};			                  //indexing variable
		 char *plain_text{ new char [new_length + 1]{}};	  // holds the converted plaintext


		 /* takes the first 5 characters and converts from
		  * base 85 to base 10
		  */
		 for (std::size_t q{0}; q < length/5; ++q){
		 		for (std::size_t r{0}; r <= 4; ++r){
		 			char current{ciphertext[counter]};
		 			sum += ((current - '!')*pow(85,4-r));
		 			//increases the index counter
		 			++counter;
		 		}


		 		/* uses bitshift operations to extract the 4
		 		 * characters from the sum, this creates the ciphertext
		 		 */
		 		plain_text[4*q] = sum >> 24 % 256;
		 		plain_text[4*q + 1] = sum >> 16 % 256;
		 		plain_text[4*q + 2] = sum >> 8 % 256;
		 		plain_text[4*q + 3] = sum % 256;

		 		//resets the sum for the next set of numbers
		 		sum = 0;
		 }

		 //XOR with R to convert the cipher text back to plaintext
		 for (std::size_t p{}; p < new_length; ++p){
		 		i = ( i + 1) % 256;
		 		j = ( j + S[i]) % 256;
		 		unsigned char temp2{S[j]};
		 		S[j] = S[i];
		 		S[i] = temp2;
		 		int r = (S[i] + S[j]) % 256;
		 		int R = S[r];
		 		plain_text[p] = (plain_text[p] ^ R);
		 	}

		//add the null character at the end to signify end of string
		 plain_text[new_length] = '\0';

	return plain_text;

}

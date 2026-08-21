#include <iostream>
#include <string>
#include <cstring>
#include "encode.h"
#include "decode.h"
#include "types.h"


OperationType EncodeInfo::check_operation_type(char** argv)
{
    if(strcmp(argv[1],"-e")==0)      
	return e_encode;
    else if(strcmp(argv[1],"-d")==0)  
	return e_decode;
    else
	return e_unsupported;
}

int main(int argc,char*argv[])
{
    
       EncodeInfo encInfo;
       DecodeInfo decInfo;

    
    if(argc>=3)
    {
	if((encInfo.check_operation_type(argv))==e_encode)        
	{
	    printf("Selected Encoding\n");

	   
	    if(encInfo.read_and_validate_encode_args(argv)==e_failure)
	    {
		printf("Unsuccessful reading and validating\n");
	    }
	    else
	    {
		printf("Successful reading and validating\n");
		
		
		   if(encInfo.do_encoding()==e_success)         
		   printf("Encoding Successful\n");
		   else
		   printf("ENcoding failure\n");
		   
		 
	    }

	}
	else if((encInfo.check_operation_type(argv))==e_decode)     
	{
	    printf("Selected Decoding\n");
        
	    if(decInfo.read_and_validate_decode_args(argv)==e_success)
	    {
		printf("Successfull reading and validation\n");
		
		   if(decInfo.do_decoding()==e_success)        
		   printf("Decoding Successful\n");
		   else
		   printf("Decoding unsuccessful");
		 
	    }
	    else
		printf("UNsuccessful reading and validating\n");
		


	}
	else                                               
	    printf("Error:Entered option is not valid\n");
	    
    }
    else
	printf("Error:Enter Cmd line inputs more than 3 or equal to 3\n");

}

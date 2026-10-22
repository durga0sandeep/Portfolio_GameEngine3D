#version 330 core
out vec4 FragColor;    // final output color

uniform int Flag;

void main()
{

	if(Flag == 0){

		FragColor = vec4(1.0,0.0,0.0, 1.0); 
		
	}else if(Flag == 1){
		
		FragColor = vec4(0.0,1.0,0.0, 1.0); 

	}else if(Flag == 2){
			
		FragColor = vec4(0.0,0.0,1.0, 1.0); 
	}else{
   		
		FragColor = vec4(1.0,1.0,1.0, 1.0); 
	
	}
    
}

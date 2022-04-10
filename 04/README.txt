Rachel Ng 

image.* 
Image class provided by Professor Stamos 




s1.cc 

USAGE: ./s1 input_image threshold output_file


s2.cc

USAGE: ./s2 parameters_file image_1 image_2 image_3 directions_file

formulas 
x = pixel.x - center.x 
y = pixel.y - center.y 
z = sqrt(r^2 - x^2 - y^2)
direction magnitude = x^2 + y^2 + z^2 


s3.cc 

USAGE: ./s3 input_directions image_1 image_2 image_3 step threshold output

    step = 10 
    threshold = 85 


s4.cc 

USAGE: ./s4 input_directions image_1 image_2 image_3 threshold output

    threshold = 85 




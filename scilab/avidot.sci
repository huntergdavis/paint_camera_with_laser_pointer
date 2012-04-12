//"paint" a scene with a laser pointer in realtime
//speed of update depends on speed of camera + processor
//could also easily be used on a video file
n = aviopen("video.avi");
for idx=1:15,			//give the camera time to auto white balance
	im1=avireadframe(n);
end;
im3prime = rgb2gray(im1);	//save our "primary scene"
im3 = im3prime;			
imshow(im3);				//show us our primary scene

for ido=1:15,
	mask = zeros(im3);			//clear out our mask quickly
	for idx=1:3,				//or however many frames/sec you can process
		//tic;
		im2=avireadframe(n);		//read a frame in
	
		//subtract the greyscale current image from the primary scene
		//then take that logical array, convert to numerical and
		//use it as a mask over im3
		mask = mask + bool2s(imsubtract(rgb2gray(im2), im3prime) > 50);
		//imshow(im3);
		//toc
	end;
	im3(mask == 1) = 255;
	imshow(im3);
end;

avicloseall();
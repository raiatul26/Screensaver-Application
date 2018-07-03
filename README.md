/////////////////////////////////////////////////*****************************\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                                  
                                                    N - Balls Screensaver
				*********              Assighment - 1               **********
							 COP 290 

\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\******************************//////////////////////////////////////////////////////////

Submission  Date : 19th February 2018 
Instructor : Prof. Huzur Saran 

Submitted By : Atul Rai      (2016CSJ0074)
	       Dwij Upadhyay (2016CSJ0009)

****************************************************************************************************************************************
ABOUT:
The purpose of this project is to create an interactive Screensaver, which includes 3-D collision of n-balls(specified by user) with each-other inside a box containing moving terrain and also includes the interaction of application with the user using controls like Pause, IncreaseSpeed, DecreaseSpeed. The purpose of this project was to learn and get familiar and comfortable with GLUT library and pthreads.

****************************************************************************************************************************************


COMPILATION AND EXECUTION: 
1. Clone the repository.
2. Change Directory to ScreensaverApp.
3. Open terminal and Execute command "make execute num_thread=n" where in place of n input the number of balls you want in your box.
4. If you will enter "make" on command line then 12 balls will be displayed on the screen.

****************************************************************************************************************************************


REQUIREMENTS:

    Libraries: 
    
	1. openGL
	2. glut or free glut
	3. pthread
	The version of g++ required to compile c++11


****************************************************************************************************************************************

USER INTERFACE:
 
    KEYBOARD CONTROLS:

    The following is a list of the KeyBoard functions available:

	• Number of Balls : First of all the user has power to enter(input) any number of balls (<=60), that he or she want to enter into the  box. 
	
	• Increase Speed : '+' allow the user to increase the speed of a selected Ball.
	
	• Decrease Speed : '-' allow the user to decrease the speed of a selected Ball.
	
	• Selection : A ball can be selected using 'a' and 'd' keyboard keys. When a ball is selected its colour will get changed to white so that it can be easily determined which ball is selected by user. 
	
	• Pause/Play : Space Bar allows the user to pause the simulation, and resume the same, as per his convenience. And one more thing, you  can observe that as we pause or play using spacebar, the GUI Button's text will also get changed to "Play" or "Pause".  Isn't it interesting!
	
	• Toggle Selection : g allows the user to enable/disable the effects of gravity within the Box.
	
	• Camera Controls : The Up, Down, Left and Right arrow keys allow the user to change the view of the camera, allowing the user to view the Box, and the Balls from different angles,while facilitating the selection of Balls. Up and Down arrow keys will move the camera in and out of the box respectively whereas Left and Right arrow keys allow user to left and right around the box.
	
	• Change Shape: We have additionally added the the functionality for changing the shape of our moving balls using the keyboard keys 's' and 'w'. This functionality is just for fun and if still is less then we have another functionality of changing the shape of our terrain using keys 'f', 'h', 't' and 'g'.
	
	
	MOUSE CONTROLS:

	• Decrease Speed : Sometimes using the keyboard controls can make you fall in some trouble. It's because sometimes you may increase the speed of the particular ball too much that it transfers its speed to other balls getting you in trouble as the speed of all the balls will eventually increase to a heck. So, for the same, we have designed a GUI Button '-' which will help you taking the speed of all the balls down to a safe value so that you can still enjoy this application without any issue.


*****************************************************************************************************************************************


	HOW TO USE:

 	--- Compile the program using the command given in the above section.
 
	 --- Input the number of balls to be displayed on the screen. 
 
 	--- Now using the controls given in the above section operate this awesome screensaver application.

***************************************************************************************************************************************** 

BIBLIOGRAPHY:
	
	OpenGL SuperBible Book
	StackOverflow
 	gamedev.net
	khronos.org
	codeyarns.com
	OpenGL documentation 
	bogotobogo.net/cplusplus/*
	Lighthose3d.com
	OpenGL.sourceforge.net
	Wikipedia

# COMP4411P4
COMP4411 Project 4 Animator

Requirements
Here is a summary of the requirements for this project:
Implement the following curve types with support for wrapping:
- [X] Bézier (splined together with C0 continuity)
- [X] B-spline
- [X] Catmull-Rom
- [] Implement a particle system that is integrated with your model in some way.
Bells and Whistles
 [] :notes: Enhance the required spline options. Some of these will require alterations to the user interface, which is somewhat complicated to understand.  If you want to access mouse events in the graph window, look at the handle function in the GraphWidget class.  Also, look at the Curve class to see what control point manipulation functions are already provided.  These could be helpful, and will likely give you a better understanding of how to modify or extend your program's behavior.  A maximum of 3 whistles will be given out in this category.

- [X] Let the user control the tension of the Catmull-Rom spline
- [] Implement higher degree polynomial splines (ones that are C3 or C4 continuous)
- [] Implement one of the standard subdivision curves (e.g., Lane-Riesenfeld or Dyn-Levin-Gregory).
- [] Allow the user to specify the derivatives at the two endpoints of your C2 interpolating curves.
- [] Add options to the user interface to enforce C0 or C1 continuity between adjacent Bézier curve segments automatically. (It should also be possible to override this feature in cases where you don't want this type of continuity.)
- [] Add the ability to add a new control point to any curve type without changing the curve at all.
 [] :notes: A TGA file (Long description) is a file format commonly used graphic file format that is easy to read and write. Extends your project so that it support the use of the transparent TGA files. Transparent TGA files is a  32bit TGA files that have a alpha mask included in the file. These images can be used to create complex looking objects while they are only drawn on a single quad. eg. Trees and fences. This example also demos the use of fog in a scene and contains a TGA parser (example).

[] :notes: Render a mirror in your scene. As you may already know, OpenGL has no built-in reflection capabilities. You can simulate a mirror with the following steps: 1) Reflect the world about the mirror's plane, 2) Draw the reflected world, 3) Pop the reflection about the mirror plane from your matrix stack, 4) Draw your world as normal.  After completing these steps, you may discover that some of the reflected geometry appears outside the surface of the mirror.  For an extra whistle you can clip the reflected image to the mirror's surface, you need to use something called the stencil buffer.  The stencil buffer is similar to a Z buffer and is used to restrict drawing to certain portions of the screen.
[] :notes: Particles rendered as points or spheres may not look that realistic.  You can achieve more spectacular effects with a simple technique called billboarding.  A billboarded quad (aka "sprite") is a textured square that always faces the camera.  See here for a demo.  For full credit, you should load a texture with transparency (some sample textures are available here), and turn on alpha blending Hint:  When rotating your particles to face the camera, it's helpful to know the camera's up and ri ght vectors in world-coordinates.

[] :notes: Use the billboarded quads you implemented above to render the following effects.  Each of these effects is worth one whistle provided you have put in a whistle worth of effort making the effect look good.

 - Fire (example) (You'll probably want to use additive blending for your particles - glBlendFunc(GL_SRC_ALPHA,GL_ONE); )
 - Snow
 - Water fountain
 - Fireworks
[] :notes: Use environment mapping to simulate a reflective material.  This technique is particularly effective at faking a metallic material or reflective, rippling water surface.  Note that OpenGL provides some very useful functions for generating texture coordinates for spherical environment mapping.  Part of the challenge of this whistle is to find these functions and understand how they work.  
[] :notes:  Implement a motion blur effect (example1, example2).  The easy way to implement motion blur is using an accumulation buffer - however, consumer grade graphics cards do not implement an accumulation buffer.  You'll need to simulate an accumulation buffer by rendering individual frames to a texture, then combining those textures.

 [] :notes: Implement a skyboxes (example) for rendering background of scene. It works by projecting images of the environment onto the inside of a box which surrounds the viewing position. Sometimes other shapes are used, i.e. sky domes or sky spheres. When done correctly, it creates an almost perfect illusion. A tutorial of rendering skyboxes is here.
 
[] :notes: Euler's method is a very simple technique for solving the system of differential equations that defines particle motion.  However, more powerful methods can be used to get better, more accurate results.  Implement your simulation engine using a higher-order method such as the Runge-Kutta technique.  See here (Numerical Recipes, Sections 16.0, 16.1) for a description of Runge-Kutta and pseudo-code.

[] :notes: Use openGL  display lists to speed up the rendering of one or more of your more complicated models.  Display lists allow you to render complicated polygons much more quickly by storing more information directly on the graphics card. That way much less informaiton is sent across the (slow) computer bus for each frame rendered.  A display list tutorial can be found here.

[] :notes: Implement adaptive Bézier curve generation; i.e., use a recursive, divide-and-conquer, de Casteljau algorithm to produce Bézier curves, rather than just sampling them at some arbitrary interval. You are required to provide some way to change these variables, with a keystroke or mouse click.  In addition, you should have some way of showing (a printf statement is fine) the number of points generated for a curve to demonstrate your adaptive algorithm at work.  If you provide visual controls to toggle the feature, modify the flatness parameter (with a slider for e.g.) and show the number of points generated for each curve, you will get an extra whistle.

[] :bell: Extend the particle system to handle springs. For example, a pony tail can be simulated with a simple spring system where one spring endpoint is attached to the character's head, while the others are floating in space.  In the case of springs, the force acting on the particle is calculated at every step, and it depends on the distance between the two endpoints.  For one more bell, implement spring-based cloth.  For 2 more bells, implement spring-based fur.  The fur must respond to collisions with other geometry and interact with at least two forces like wind and gravity.

[] :bell: Allow for particles to bounce off each other by detecting collisions when updating their positions and velocities.  Although it is difficult to make this very robust, your system should behave reasonably.  

[] :bell: Implement a "general" subdivision curve, so the user can specify an arbitrary averaging mask  You will receive still more credit if you can generate, display, and apply the evaluation masks as well.  There's a site at Caltech with a few interesting applets that may be useful.

 [] :bell: Heightfields (example) are great ways to build complicated looking maps and terrains pretty easily.

 [] :bell: If you have done metaballs in project 2 or 3.  Extend your metaballs so that it support environment mapping.(example). 3 more bells will be given if you haven't done metaballs before.

[] :bell: :notes:Add a lens flare.  This effect has components both in screen space and world space effect.  For full credit, your lens flare should have at least 5 flare "drops", and the transparency of the drops should change depending on how far the light source is from the center of the screen.  You do not have to handle the case where the light source is occluded by other geometry (but this is worth an extra whistle).

 [] :bell: :notes:If you find something you don't like about the interface, or something you think you could do better, change it! Any really good changes will be incorporated into Animator 2.0.  Credit varies with the quality of the improvement.

[] :bell: :bell: Add flocking behaviors to your particles to simulate creatures moving in flocks, herds, or schools.  A convincing way of doing this is called "boids"  (see here for a demo and for more information).  For full credit, use a model for your creatures that makes it easy to see their direction and orientation (for example, the yellow/green pyramids in the boids demo would be a minimum requirement).  For up to one more bell, make realistic creature model and have it move realistically according to its motion path.  For example, a bird model would flap its wings when it rises, and hold it's wings outstretched when turning.

[] :bell: :bell: Implement a C2-Interpolating curve.  There is already an entry for it in the drop-down menu.

 [] :bell: :bell: Add the ability to edit Catmull-Rom curves using the two "inner" Bézier control points as "handles" on the interpolated "outer" Catmull-Rom control points. After the user tugs on handles, the curve may no longer be Catmull-Rom.  In other words, the user is really drawing a C1 continuous curve that starts off with the Catmull-Rom choice for the inner Bézier points, but can then be edited by selecting and editing the handles.  The user should be allowed to drag the interpolated point in a manner that causes the inner Bézier points to be dragged along.  See PowerPoint and Illustrator pencil-drawn curves for an example.

 [] :bell: :bell: Implement picking of a part in the model hierarchy.  In other words, make it so that you can click on a part of your model to select its animation curve.  To recognize which body part you're picking, you need to first render all body parts into a hidden buffer using only an emissive color that corresponds to an object ID.  After modifying the mouse-ing UI to know about your new picking mode, you'll figure out which body part the user has picked by reading out the ID from your object ID buffer at the location where the mouse clicked.  This should then trigger the GraphWidget to select the appropriate curve for editing.  If you're thinking of doing either of the inverse kinematics (IK) extensions below, this kind of interface would be required.

[] :bell: :bell: If you implemented twist for your original model, the camera movement for your old modeler can give some unexpected results.  For example, twist your model 90 degrees.  Now try to do rotations as normal.  This effect is called gimbal lock and we'll likely discuss it in class.  Implement Quaternions as a method for avoiding the gimbal lock.

 [] :bell: :bell: In class, your learnt how to draw different kind of curve, extend the basic requirements so that it can draw bspline surface in the 3D scene. In this extension, you are required to allow user to adjust the control points.

[] :bell: :bell: :bell:Implement projected textures (example).  Projected textures are used to simulate things like a slide projector, spotlight illumination, or casting shadows onto arbitrary geometry.  Check out this demo and read details of the effect at glBase, and SGI. 1 extra bell will be given if your also do projected shadows (example).

 [] :bell: :bell: :bell:An alternative way to do animations is to transform an already existing animation by way of motion warping (animations). Extend the animator to support this type of motion editing.

[] :bell: :bell: :bell:Implement Cell Shading (example), also know as Cartoon Rendering. Cell Shading is a very powerful form of rendering, and it's results can completely change the "feel" of a rendering.

[] :bell: :bell: :bell: :bell:We've talked about rigid-body simulations in class.  Incorporate this functionality into your program, so that you can correctly simulate collisions and response between rigid objects in your scene.  You should be able to specify a set of objects in your model to be included in the simulation, and the user should have the ability to enable and disable the simulation either using the existing "Simulate" button, or with a new button.   

 [] :bell: :bell: :bell: :bell:Extend your system to support subdivision surfaces.   Provide a simple interface for the user to edit a surface.  The user should also be able to specify surface features that stay constant so that sharp creases can be formed.  Tie your surface to the animation curves to demonstrate a dynamic scene.  As mentioned above in the blurb for the subdivision curve bell, Caltech has a few nice applets here.

Disclaimer: please consult the course staff before spending any serious time on these. They are quite difficult, and credit can vary depending on the quality of your method and implementation.
                

Inverse kinematics
The hierarchical model that you created is controlled by forward kinematics; that is, the positions of the parts vary as a function of joint angles. More mathematically stated, the positions of the joints are computed as a function of the degrees of freedom (these DOFs are most often rotations). The problem is inverse kinematics is to determine the DOFs of a model to satisfy a set of positional constraints, subject to the DOF constraints of the model (a knee on a human model, for instance, should not bend backwards).

This is a significantly harder problem than forward kinematics. Aside from the complicated math involved, many inverse kinematics problems do unique solutions. Imagine a human model, with the feet constrained to the ground. Now we wish to place the hand, say, about five feet off the ground. We need to figure out the value of every joint angle in the body to achieve the desired pose. Clearly, there are an infinite number of solutions. Which one is "best"?

Now imagine that we wish to place the hand 15 feet off the ground. It's fairly unlikely that a realist ic human model can do this with its feet still planted on the ground. But inverse kinematics must provide a good solution anyway. How is a good solution defined?

Your solver should be fully general and not rely on your specific model (although you can assume that the degrees of freedom are all rotational). Additionally, you should modify your user interface to allow interactive control of your model though the inverse kinematics solver. The solver should run quickly enough to respond to mouse movement.

If you're interested in implementing this, you will probably want to consult the following lecture notes.

                

Interactive Control of Physically-Based Animation
Create a character whose physics can be controlled by moving a mouse or pressing keys on the keyboard.  For example, moving the mouse up or down may make the knees bend or extend the knees (so your character can jump), while moving it the left or right could control the waist angle (so your character can lean forward or backward).  Rather than have these controls change joint angles directly, as was done in the modeler project, the controls should create torques on the joints so that the character moves in very realistic ways.  This monster bell requires components of the rigid body simulation extension above, but you will receive credit for both extensions as long as both are fully implemented..  For this extension, you will create a hierarchical character composed of several rigid bodies.   Next, devise a way user interactively control your character.  

This technique can produce some organic looking movements that are a lot of fun to control.  For example, you could create a little Luxo Jr. that hops around and kicks a ball.  Or, you could create a downhill skier that can jump over gaps and perform backflips (see the Ski Stunt example below).

SIGGRAPH paper - http://www.dgp.toronto.edu/~jflaszlo/sig2000.pdf

Several movie examples - http://www.dgp.toronto.edu/~jflaszlo/interactive-control.html

If you want, you can do it in 2D, like the examples shown in this paper (in this case you will get full monster bell credit, but half credit for the rigid body component).
# AnimatedDog
C app using OpenGL that builds and moves a simple modular hierarchical model of dog.
The dog consists of a torso, a tail, 4 legs, which consist of an upper and lower limb, a foot, a neck and a head. All of these parts are modelled as cylinders with gluCylinder(),
apart from the head which is modeled as a sphere with gluSPhere().
The dog is still by default and the camera view is set to the side.

There are 7 menu options:
The first makes the dog bent its front leg.
The second makes the dog lift on its hind legs.
The third makes the dog bend its neck, which in turn moves its head as well.
The fourth makes the dog wiggle its tail continuously.
The fifth makes the dog reset back to its default state.
The sixth changes the camera view from the side to the front and vice versa.
The seventh one shuts down the application.

Whenever a new animation is selected, the dog first resets to its default state, before starting the selected animation.

The app uses double buffering, and depth buffering.

NOTE: If using Microsoft Visual Studio make sure that the Active solution platform is x86 (Build -> Configuration Manager).

# The plan

## Scene Files

Scene files will be layed out in a simple format  
[ItemID]$X;Y;Z  
ItemID is a unique ID that belongs to an item. For sake of not overwriting items  
users will have no control over the ID and the ID will be strictly numerical  
  
X Y And Z seprated by a ; will define where **THE CENTER** of the object lies.  
This is important for centering objects exactly correctly. 

## Objects

Object files will contain 2 lists seperated by a $ Symbol for each list and a  
; for each entry. These will hold in order a list of the vertexes needed in world  
space and then a list of indicies needed to create the mesh. These indices will  
be offset at runtime based on what items are loaded in before it. These files  
will also contain shader IDs.

## Shaders
Shaders will be built using raw text in .fs or .vs files for fragment and vertex shaders  
these files will have a unique id choosen by the author which will contain useful info  
such as the usage of the shader. Example: Opacitiy_Shader_50. It is the job of the  
object author to warn the player to any shaders they may need. Shaders will be loaded  
into memory before hand with thier unique ID being a name for the objects to read

## Putting it all together
When a scene is loaded it should read the file and for every item it has not seen  
it should find said object in memory. The object will be loaded into memory at start  
and its data will be copied int one large buffer. Before the data is copied any  
offsets should be applied to the indicies to avoid selecting other points from   
pprevious files. A center point will be calculated for each object and said center  
will be  used in the scene file to determine exactly where to place the object.  
the shaders will be applied to the objects and multiple "one large buffers"  
may be needed to support multiple shaders.
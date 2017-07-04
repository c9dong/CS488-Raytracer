-- A simple scene with some miscellaneous geometry.
-- This file is very similar to nonhier.lua, but interposes
-- an additional transformation on the root node.  
-- The translation moves the scene, and the position of the camera
-- and lights have been modified accordingly.

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25)
mat5 = gr.texture('Assets/basketball.png')

scene = gr.node( 'scene' )
scene:translate(10, 0, -15)

-- u = gr.union('union')
-- scene:add_child(u)
-- u:translate(-200, 0, -400)

s1 = gr.cube('s1')
scene:add_child(s1)
s1:set_material(mat5)
s1:scale(2, 3, 1);
s1:translate(-5, -5, 0);

-- s2 = gr.nh_sphere('s2', {0, 0, -0}, 100)
-- u:add_child(s2)
-- s2:set_material(mat1)

-- i = gr.intersect('intersect')
-- scene:add_child(i)
-- i:rotate('Y', 50)
-- i:translate(0, 0, -400)

-- s4 = gr.nh_sphere('s4', {0, 0, -0}, 100)
-- i:add_child(s4)
-- s4:set_material(mat1)

-- s3 = gr.nh_box('s3', {-50, -100, 0}, 100)
-- i:add_child(s3)
-- s3:set_material(mat3)
-- s3:scale(5, 0.5, 5)
-- s3:rotate('X', 90)
-- s3:translate(-150, 0, 200)

-- d = gr.difference('difference')
-- scene:add_child(d)
-- d:rotate('Y', 50)
-- d:translate(200, 0, -400)

-- s6 = gr.nh_sphere('s6', {0, 0, -0}, 100)
-- d:add_child(s6)
-- s6:set_material(mat1)

-- s5 = gr.nh_box('s5', {-50, -50, 0}, 100)
-- d:add_child(s5)
-- s5:set_material(mat1)
-- s5:scale(2, 0.5, 1)
-- s5:translate(0, 0, -100)

-- s3 = gr.nh_sphere('s3', {0, -1200, -500}, 1000)
-- scene:add_child(s3)
-- s3:set_material(mat2)

-- b1 = gr.nh_box('b1', {-200, -125, 0}, 100)
-- scene:add_child(b1)
-- b1:set_material(mat4)

-- s4 = gr.nh_sphere('s4', {-100, 25, -300}, 50)
-- scene:add_child(s4)
-- s4:set_material(mat3)

-- s5 = gr.nh_sphere('s5', {0, 100, -250}, 25)
-- scene:add_child(s5)
-- s5:set_material(mat1)

-- A small stellated dodecahedron.

-- steldodec = gr.mesh( 'dodec', 'smstdodeca.obj' )
-- steldodec:set_material(mat3)
-- scene:add_child(steldodec)

white_light = gr.light({-100.0, 150.0, -400.0}, {0.5, 0.5, 0.5}, {1, 0, 0})
orange_light = gr.light({400.0, 100.0, -650.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene, 'nonhier.png', 1000, 1000,
	  {0, 0, 0}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light})

-- A simple scene with some miscellaneous geometry.

mat1 = gr.material({0, 0, 0}, {0, 0, 0}, 25)
mat2 = gr.material({0.4, 0.9, 0.2}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25)
mat4 = gr.material({1.0, 0.6, 0.0}, {0.5, 0.4, 0.8}, 50)

scene_root = gr.node('root')

-- s1 = gr.nh_sphere('s1', {0, 0, -400}, 100)
-- scene_root:add_child(s1)
-- s1:set_material(mat1)

-- c = gr.nh_cylinder('c1', {100, 50, -200}, 100, 300);
-- scene_root:add_child(c)
-- c:set_material(mat3)

-- s2 = gr.nh_sphere('s2', {100, 50, -200}, 150)
-- scene_root:add_child(s2)
-- s2:set_material(mat1)

c3 = gr.nh_cone('c3', {-200, 150, -200}, 100, 300)
scene_root:add_child(c3)
c3:set_material(mat3)

s1 = gr.nh_sphere('s1', {-200, 150, -200}, 120)
scene_root:add_child(s1)
s1:set_material(mat3)

-- s3 = gr.nh_sphere('s3', {0, -1200, -200}, 1000)
-- scene_root:add_child(s3)
-- s3:set_material(mat2)

s2 = gr.nh_sphere('s2', {-200, 30, -200}, 100)
scene_root:add_child(s2)
s2:set_material(mat3)

-- s3 = gr.nh_sphere('s3', {0, -1200, -500}, 1000)
-- scene_root:add_child(s3)
-- s3:set_material(mat2)

-- b1 = gr.nh_box('b1', {-200, -125, 0}, 100)
-- scene_root:add_child(b1)
-- b1:set_material(mat4)

-- s4 = gr.nh_sphere('s4', {-100, -170, 300}, 30)
-- scene_root:add_child(s4)
-- s4:set_material(mat1)

-- s5 = gr.nh_sphere('s5', {-150, -170, 200}, 30)
-- scene_root:add_child(s5)
-- s5:set_material(mat1)

-- s6 = gr.nh_sphere('s6', {-134, -170, 350}, 30)
-- scene_root:add_child(s6)
-- s6:set_material(mat1)

-- s7 = gr.nh_sphere('s7', {100, -170, 250}, 30)
-- scene_root:add_child(s7)
-- s7:set_material(mat1)

-- s8 = gr.nh_sphere('s8', {150, -170, 300}, 30)
-- scene_root:add_child(s8)
-- s8:set_material(mat1)

-- s5 = gr.nh_sphere('s5', {0, 100, -250}, 25)
-- scene_root:add_child(s5)
-- s5:set_material(mat1)

-- A small stellated dodecahedron.

steldodec = gr.mesh( 'dodec', 'smstdodeca.obj' )
steldodec:set_material(mat3)
scene_root:add_child(steldodec)

white_light = gr.light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
orange_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene_root, 'nonhier.png', 1000, 1000,
	  {0, 0, 800}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light, orange_light})

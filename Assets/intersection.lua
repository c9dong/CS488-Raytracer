-- A simple scene with some miscellaneous geometry.
-- This file is very similar to nonhier.lua, but interposes
-- an additional transformation on the root node.  
-- The translation moves the scene, and the position of the camera
-- and lights have been modified accordingly.

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25)

scene = gr.node( 'scene' )
scene:translate(0, 0, -800)

u1 = gr.union('union1')
scene:add_child(u1)
u1:scale(0.5, 0.5, 0.5)
u1:rotate('X', 20)
u1:translate(0, -300, 0)

s1 = gr.nh_sphere('s1', {0, 0, 0}, 200)
u1:add_child(s1)
s1:set_material(mat1)
s1:scale(1, 0.25, 1)

s2 = gr.nh_sphere('s2', {0, 0, 0}, 100)
u1:add_child(s2)
s2:set_material(mat3)

i1 = gr.intersect('intersect1')
scene:add_child(i1)
i1:scale(0.5, 0.5, 0.5)
i1:rotate('X', 20)
i1:translate(-300, -300, 0)

s3 = gr.nh_sphere('s3', {0, 0, 0}, 200)
i1:add_child(s3)
s3:set_material(mat1)
s3:scale(1, 0.25, 1)

s4 = gr.nh_sphere('s4', {0, 0, 0}, 100)
i1:add_child(s4)
s4:set_material(mat3)

d1 = gr.difference('difference')
scene:add_child(d1)
d1:scale(0.5, 0.5, 0.5)
d1:translate(300, -300, 0)

s5 = gr.nh_sphere('s5', {0, 0, 0}, 200)
d1:add_child(s5)
s5:set_material(mat1)
s5:scale(1, 0.25, 1)

s6 = gr.nh_sphere('s6', {0, 0, 0}, 100)
d1:add_child(s6)
s6:set_material(mat3)

u2 = gr.union('union2')
u2:rotate('X', 70)
d2 = gr.difference('difference2')
d3 = gr.difference('difference3')
sr1 = gr.nh_sphere('sr1', {0,0,0}, 125)
sr1:set_material(mat1)
sr2 = gr.nh_sphere('sr2', {0,0,0}, 150)
sr2:set_material(mat1)
sr2:scale(1, 0.4, 1)
sr3 = gr.nh_sphere('sr3', {0,0,0}, 200)
sr3:set_material(mat3)
sr3:scale(1, 0.1, 1)
sr4 = gr.nh_sphere('sr4', {0,0,0}, 75)
sr4:set_material(mat1)

d3:add_child(sr2)
d3:add_child(sr1)

d2:add_child(sr3)
d2:add_child(d3)

u2:add_child(sr4)
u2:add_child(d2)

scene:add_child(u2)


white_light = gr.light({-100.0, 150.0, -400.0}, {0.5, 0.5, 0.5}, {1, 0, 0})
orange_light = gr.light({400.0, 100.0, -650.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene, 'intersect.png', 500, 500,
    {0, 0, 0}, {0, 0, -1}, {0, 1, 0}, 50,
    {0.3, 0.3, 0.3}, {white_light})

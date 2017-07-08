-- csg testing script

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25)
mat2 = gr.material({0.1, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25)
mat5 = gr.cube_texture('Assets/basketball.png')
mat6 = gr.sphere_texture('Assets/world.png')

red_mat = gr.material({1.0, 0.0, 0.0}, {0.5, 0.5, 0.5}, 25);
green_mat = gr.material({0.0, 1.0, 0.0}, {0.5, 0.5, 0.5}, 25);
blue_mat = gr.material({0.0, 0.0, 1.0}, {0.5, 0.5, 0.5}, 25);
grey_mat = gr.material({0.9, 0.9, 0.9}, {0.5, 0.5, 0.5}, 25);
yellow_mat = gr.material({1, 1, 0}, {0.5, 0.5, 0.5}, 25);
silver_mat = gr.material({0.75, 0.75, 0.75}, {0.75, 0.75, 0.75}, 75);

trans_mat = gr.transparent_material(0.9, 0.1, 1.2);

scene = gr.node( 'scene' )
scene:translate(0, 0, -15)

left_wall = gr.cube('left_wall')
scene:add_child(left_wall)
left_wall:set_material(red_mat)
left_wall:scale(0.1, 10, 20)
left_wall:translate(-6.5, 0, 0)

right_wall = gr.cube('right_wall')
scene:add_child(right_wall)
right_wall:set_material(blue_mat)
right_wall:scale(0.1, 10, 20)
right_wall:translate(6.5, 0, 0)

top_wall = gr.cube('top_wall')
scene:add_child(top_wall)
top_wall:set_material(green_mat)
top_wall:scale(20, 0.1, 20)
top_wall:translate(0, 5, 0)

bot_wall = gr.cube('bot_wall')
scene:add_child(bot_wall)
bot_wall:set_material(grey_mat)
bot_wall:scale(20, 0.1, 20)
bot_wall:translate(0, -5, 0)
    
back_wall = gr.cube('back_wall')
scene:add_child(back_wall)
back_wall:set_material(grey_mat)
back_wall:scale(20, 20, 0.1)
back_wall:translate(0, 0, -10)

-- union
union1 = gr.union('union1')
scene:add_child(union1)
union1:translate(-4, 2, -2)

cube1 = gr.cube('cube1')
cube1:scale(1.5,1.5,1.5)
cube1:set_material(red_mat)

sphere1 = gr.sphere('sphere1')
sphere1:set_material(blue_mat)

union1:add_child(cube1)
union1:add_child(sphere1)

-- intersect
intersect1 = gr.intersect('intersect1')
scene:add_child(intersect1)
intersect1:translate(4, 2, -2)

cube2 = gr.cube('cube2')
cube2:scale(1.5,1.5,1.5)
cube2:set_material(red_mat)

sphere2 = gr.sphere('sphere2')
sphere2:set_material(blue_mat)

intersect1:add_child(cube2)
intersect1:add_child(sphere2)

-- difference
difference1 = gr.difference('difference1')
scene:add_child(difference1)
difference1:translate(0, 2, -2)

cube3 = gr.cube('cube3')
cube3:scale(1.5,1.5,1.5)
cube3:set_material(red_mat)

sphere3 = gr.sphere('sphere3')
sphere3:set_material(blue_mat)

difference1:add_child(cube3)
difference1:add_child(sphere3)

-- complex
difference2 = gr.difference('difference2')
difference3 = gr.difference('difference3')

sphere3 = gr.sphere('sphere3')
sphere3:scale(3, 0.5, 3)
sphere3:set_material(yellow_mat)

cube3 = gr.cube('cube3')
cube3:scale(8, 0.5, 8)
cube3:translate(0, -0.5, 0)
cube3:set_material(yellow_mat)

cube4 = gr.cube('cube4')
cube4:scale(8, 0.5, 8)
cube4:translate(0, 0.5, 0)
cube4:set_material(yellow_mat)

difference2:add_child(sphere3)
difference2:add_child(cube3)

difference3:add_child(difference2)
difference3:add_child(cube4)

-- sharp 1
difference4 = gr.difference('difference4')
difference4:scale(1.5, 1.5, 1.5)
difference4:rotate('Y', -75)
difference4:translate(-1, 0, 2)

sphere5 = gr.sphere('sphere5')
sphere5:set_material(yellow_mat)

sphere6 = gr.sphere('sphere6')
sphere6:translate(-0.5, 0, 0)
sphere6:set_material(yellow_mat)

difference4:add_child(sphere5)
difference4:add_child(sphere6)

difference5 = gr.difference('difference5')
difference5:add_child(difference3)
difference5:add_child(difference4)

-- sharp 2
difference6 = gr.difference('difference6')
difference6:scale(1.5, 1.5, 1.5)
difference6:rotate('Y', -75)
difference6:translate(-1, 0, 2)
difference6:rotate('Y', -90)

sphere7 = gr.sphere('sphere7')
sphere7:set_material(yellow_mat)

sphere8 = gr.sphere('sphere8')
sphere8:translate(-0.5, 0, 0)
sphere8:set_material(yellow_mat)

difference6:add_child(sphere7)
difference6:add_child(sphere8)

difference7 = gr.difference('difference7')
difference7:add_child(difference5)
difference7:add_child(difference6)

-- sharp 3
difference8 = gr.difference('difference8')
difference8:scale(1.5, 1.5, 1.5)
difference8:rotate('Y', -75)
difference8:translate(-1, 0, 2)
difference8:rotate('Y', -180)

sphere9 = gr.sphere('sphere9')
sphere9:set_material(yellow_mat)

sphere10 = gr.sphere('sphere10')
sphere10:translate(-0.5, 0, 0)
sphere10:set_material(yellow_mat)

difference8:add_child(sphere9)
difference8:add_child(sphere10)

difference9 = gr.difference('difference7')
difference9:add_child(difference7)
difference9:add_child(difference8)

-- sharp 4
difference10 = gr.difference('difference10')
difference10:scale(1.5, 1.5, 1.5)
difference10:rotate('Y', -75)
difference10:translate(-1, 0, 2)
difference10:rotate('Y', -270)

sphere11 = gr.sphere('sphere11')
sphere11:set_material(yellow_mat)

sphere12 = gr.sphere('sphere12')
sphere12:translate(-0.5, 0, 0)
sphere12:set_material(yellow_mat)

difference10:add_child(sphere11)
difference10:add_child(sphere12)

difference11 = gr.difference('difference11')
difference11:add_child(difference9)
difference11:add_child(difference10)

-- cone 1
cone1 = gr.cone('cone1')
cone1:set_material(yellow_mat)
cone1:rotate('X', 180)
cone1:translate(0, 0.5, 1.75)
cone1:rotate('Y', -45)

difference12 = gr.difference('difference12')
difference12:add_child(difference11)
difference12:add_child(cone1)

-- cone 2
cone2 = gr.cone('cone2')
cone2:set_material(yellow_mat)
cone2:rotate('X', 180)
cone2:translate(0, 0.5, 1.75)
cone2:rotate('Y', -135)

difference13 = gr.difference('difference13')
difference13:add_child(difference12)
difference13:add_child(cone2)

-- cone 3
cone3 = gr.cone('cone3')
cone3:set_material(yellow_mat)
cone3:rotate('X', 180)
cone3:translate(0, 0.5, 1.75)
cone3:rotate('Y', -225)

difference14 = gr.difference('difference14')
difference14:add_child(difference13)
difference14:add_child(cone3)

-- cone 4
cone4 = gr.cone('cone4')
cone4:set_material(yellow_mat)
cone4:rotate('X', 180)
cone4:translate(0, 0.5, 1.75)
cone4:rotate('Y', -315)

difference15 = gr.difference('difference15')
difference15:add_child(difference14)
difference15:add_child(cone4)

-- center ball
union2 = gr.union('union2')
union2:rotate('X', 45)
union2:translate(0, -1.5, 0)
scene:add_child(union2)

sphere4 = gr.sphere('sphere4')
sphere4:scale(0.8, 0.8, 0.8)
sphere4:set_material(silver_mat)

union2:add_child(difference15)
union2:add_child(sphere4)


white_light_1 = gr.light({0, 4, -12}, {0.5, 0.5, 0.5}, {1, 0, 0})
white_light_2 = gr.light({0, 0, 15}, {0.5, 0.5, 0.5}, {1, 0, 0})

gr.render(scene, 'csg.png', 1000, 1000,
    {0, 0, 0}, {0, 0, -1}, {0, 1, 0}, 50,
    {0.1, 0.1, 0.1}, {white_light_1})

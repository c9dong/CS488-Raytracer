-- Primitive testing script

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

cy1 = gr.cylinder('cy1')
scene:add_child(cy1)
cy1:set_material(green_mat)
cy1:translate(-2, -4, 0)

cy2 = gr.cylinder('cy2')
scene:add_child(cy2)
cy2:set_material(green_mat)
cy2:scale(0.5, 3, 0.5)
cy2:rotate('X', 90)
cy2:rotate('Y', -35)
cy2:translate(2, -4.5, 0)

co1 = gr.cone('co1')
scene:add_child(co1)
co1:set_material(blue_mat)
co1:scale(1.5, 2.5, 1.5)
co1:translate(0, -2.5, -2)

co2 = gr.cone('co2')
scene:add_child(co2)
co2:set_material(blue_mat)
co2:rotate('X', 180)
co2:translate(2, -4, -2)


white_light_1 = gr.light({0, 4, -12}, {0.5, 0.5, 0.5}, {1, 0, 0})
white_light_2 = gr.light({0, 0, 15}, {0.5, 0.5, 0.5}, {1, 0, 0})

gr.render(scene, 'results/primitive.png', 1000, 1000,
    {0, 0, 0}, {0, 0, -1}, {0, 1, 0}, 50,
    {0.1, 0.1, 0.1}, {white_light_1}, 
    -- focal dist
    -15, 
    -- camera radius (0 for point)
    0, 
    -- camera sample rate (0 for infinite loop)
    0.2,
    -- anti aliasing sample radius (0 for no sample)
    0,
    -- anti aliasing sample rate (0 for infinite loop)
    0.5)

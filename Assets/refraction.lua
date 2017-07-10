-- Refraction testing scene

red_mat = gr.material({1.0, 0.0, 0.0}, {0.5, 0.5, 0.5}, 25);
green_mat = gr.material({0.0, 1.0, 0.0}, {0.5, 0.5, 0.5}, 25);
blue_mat = gr.material({0.0, 0.0, 1.0}, {0.5, 0.5, 0.5}, 25);
grey_mat = gr.material({0.9, 0.9, 0.9}, {0.5, 0.5, 0.5}, 25);

trans_mat = gr.transparent_material(0.9, 0.1, 1.3);

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

sphere1 = gr.sphere('sphere1')
scene:add_child(sphere1)
sphere1:set_material(trans_mat)
sphere1:scale(2, 2, 2)
sphere1:translate(1, -2, 2)

sphere2 = gr.sphere('sphere2')
scene:add_child(sphere2)
sphere2:set_material(green_mat)
sphere2:translate(0, -3, -7)

sphere3 = gr.sphere('sphere3')
scene:add_child(sphere3)
sphere3:set_material(blue_mat)
sphere3:translate(-4, -3, -7)

sphere4 = gr.sphere('sphere4')
scene:add_child(sphere4)
sphere4:set_material(red_mat)
sphere4:translate(4, -3, -7)


white_light_1 = gr.light({0, 4, -12}, {0.5, 0.5, 0.5}, {1, 0, 0})
white_light_2 = gr.light({0, 0, 15}, {0.5, 0.5, 0.5}, {1, 0, 0})

gr.render(scene, 'results/refraction.png', 500, 500,
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


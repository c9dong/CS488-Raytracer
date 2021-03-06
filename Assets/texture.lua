-- Primitive testing script

pool_mat = gr.cube_texture('Assets/pooltable2.png', 0)
wood_mat = gr.cube_texture('Assets/wood.png', 25)
mat_ball_13 = gr.sphere_texture('Assets/ball13.png', 50)
mat_ball_2 = gr.sphere_texture('Assets/ball2.png', 50)
mat_ball_15 = gr.sphere_texture('Assets/ball15.png', 50)

black_mat = gr.material({0, 0, 0}, {0.5, 0.5, 0.5}, 25);

scene = gr.node( 'scene' )
scene:translate(0, 0, -15)

pool_union_frame_1 = gr.union('pool_union_frame_1')
pool_union_frame_2 = gr.union('pool_union_frame_2')
pool_union_frame_3 = gr.union('pool_union_frame_3')
pool_union_frame_4 = gr.union('pool_union_frame_4')


pool_table = gr.cube('pool_table')
pool_table:set_material(pool_mat)
pool_table:scale(1.5, 0.5, 2.2)
pool_table:translate(0, -0.2, 0)

pool_table_frame_left = gr.cube('pool_table_frame_left')
pool_table_frame_left:set_material(wood_mat)
pool_table_frame_left:scale(0.15, 0.15, 2.2)
pool_table_frame_left:translate(-0.75, 0.1, 0)

pool_union_frame_4:add_child(pool_table)
pool_union_frame_4:add_child(pool_table_frame_left)

pool_table_frame_top = gr.cube('pool_table_frame_top')
pool_table_frame_top:set_material(wood_mat)
pool_table_frame_top:scale(1.5, 0.15, 0.2)
pool_table_frame_top:translate(0, 0.1, -1)

pool_union_frame_3:add_child(pool_union_frame_4)
pool_union_frame_3:add_child(pool_table_frame_top)

pool_table_frame_right = gr.cube('pool_table_frame_right')
pool_table_frame_right:set_material(wood_mat)
pool_table_frame_right:scale(0.15, 0.15, 2.2)
pool_table_frame_right:translate(0.75, 0.1, 0)

pool_union_frame_2:add_child(pool_union_frame_3)
pool_union_frame_2:add_child(pool_table_frame_right)

pool_table_frame_bottom = gr.cube('pool_table_frame_bottom')
pool_table_frame_bottom:set_material(wood_mat)
pool_table_frame_bottom:scale(1.5, 0.15, 0.2)
pool_table_frame_bottom:translate(0, 0.1, 1)

pool_union_frame_1:add_child(pool_union_frame_2)
pool_union_frame_1:add_child(pool_table_frame_bottom)

pool_difference_hole_1 = gr.difference('pool_difference_hole_1')
pool_difference_hole_2 = gr.difference('pool_difference_hole_2')
pool_difference_hole_3 = gr.difference('pool_difference_hole_3')
pool_difference_hole_4 = gr.difference('pool_difference_hole_4')
pool_difference_hole_5 = gr.difference('pool_difference_hole_5')
pool_difference_hole_6 = gr.difference('pool_difference_hole_6')

pool_hole_top_left = gr.cylinder('pool_hole_top_left')
pool_hole_top_left:set_material(black_mat)
pool_hole_top_left:scale(0.1, 0.15, 0.1)
pool_hole_top_left:translate(-0.65, 0.1, -0.85)

pool_difference_hole_1:add_child(pool_union_frame_1)
pool_difference_hole_1:add_child(pool_hole_top_left)

pool_hole_top_right = gr.cylinder('pool_hole_top_right')
pool_hole_top_right:set_material(black_mat)
pool_hole_top_right:scale(0.1, 0.15, 0.1)
pool_hole_top_right:translate(0.65, 0.1, -0.85)

pool_difference_hole_2:add_child(pool_difference_hole_1)
pool_difference_hole_2:add_child(pool_hole_top_right)

pool_hole_bottom_left = gr.cylinder('pool_hole_bottom_left')
pool_hole_bottom_left:set_material(black_mat)
pool_hole_bottom_left:scale(0.1, 0.15, 0.1)
pool_hole_bottom_left:translate(-0.65, 0.1, 0.85)

pool_difference_hole_3:add_child(pool_difference_hole_2)
pool_difference_hole_3:add_child(pool_hole_bottom_left)

pool_hole_bottom_right = gr.cylinder('pool_hole_bottom_right')
pool_hole_bottom_right:set_material(black_mat)
pool_hole_bottom_right:scale(0.1, 0.15, 0.1)
pool_hole_bottom_right:translate(0.65, 0.1, 0.85)

pool_difference_hole_4:add_child(pool_difference_hole_3)
pool_difference_hole_4:add_child(pool_hole_bottom_right)

pool_hole_left = gr.cylinder('pool_hole_left')
pool_hole_left:set_material(black_mat)
pool_hole_left:scale(0.1, 0.15, 0.1)
pool_hole_left:translate(-0.64, 0.1, 0)

pool_difference_hole_5:add_child(pool_difference_hole_4)
pool_difference_hole_5:add_child(pool_hole_left)

pool_hole_right = gr.cylinder('pool_hole_right')
pool_hole_right:set_material(black_mat)
pool_hole_right:scale(0.1, 0.15, 0.1)
pool_hole_right:translate(0.64, 0.1, 0)

pool_difference_hole_6:add_child(pool_difference_hole_5)
pool_difference_hole_6:add_child(pool_hole_right)


pool_table_full = gr.node( 'pool_table_full' )
pool_table_full:add_child(pool_difference_hole_6)
pool_table_full:scale(10, 10, 10)
pool_table_full:rotate('Y', -30)
pool_table_full:rotate('X', 20)
pool_table_full:translate(-1, -3, 0)

-- balls
ball13 = gr.sphere('ball13')
pool_table_full:add_child(ball13)
ball13:set_material(mat_ball_13)
ball13:scale(0.1, 0.1, 0.1)
ball13:rotate('X', 75)
ball13:rotate('Y', 14)
ball13:translate(0, 0.15, 0)

ball2 = gr.sphere('ball2')
pool_table_full:add_child(ball2)
ball2:set_material(mat_ball_2)
ball2:scale(0.1, 0.1, 0.1)
ball2:rotate('X', -22)
ball2:rotate('Y', 80)
ball2:translate(-0.1, 0.15, 0.15)

ball15 = gr.sphere('ball15')
pool_table_full:add_child(ball15)
ball15:set_material(mat_ball_15)
ball15:scale(0.1, 0.1, 0.1)
ball15:rotate('X', 15)
ball15:rotate('Z', 14)
ball15:translate(0.3, 0.15, -0.15)

scene:add_child(pool_table_full)


white_light_1 = gr.light({0, 4, -12}, {0.75, 0.75, 0.75}, {1, 0, 0})
white_light_2 = gr.light({0, 0, 15}, {0.5, 0.5, 0.5}, {1, 0, 0})

gr.render(scene, 'results/texture.png', 500, 500,
    {0, 0, 0}, {0, 0, -1}, {0, 1, 0}, 50,
    {0.1, 0.1, 0.1}, {white_light_1}, 
    -- focal dist
    -15, 
    -- camera radius (0 for point)
    0, 
    -- camera sample rate (0 for infinite loop)
    0.2,
    -- anti aliasing sample radius (0 for no sample)
    0.5,
    -- anti aliasing sample rate (0 for infinite loop)
    0.5)

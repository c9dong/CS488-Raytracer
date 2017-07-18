-- Refraction testing scene

red_mat = gr.material({1.0, 0.0, 0.0}, {0.5, 0.5, 0.5}, 25);
green_mat = gr.material({0.0, 1.0, 0.0}, {0.5, 0.5, 0.5}, 25);
blue_mat = gr.material({0.0, 0.0, 1.0}, {0.5, 0.5, 0.5}, 25);
grey_mat = gr.material({0.9, 0.9, 0.9}, {0.5, 0.5, 0.5}, 25);

trans_mat = gr.transparent_material(0.9, 0.1, 1.3);

scene = gr.node( 'scene' )

-- left_wall = gr.cube('left_wall')
-- scene:add_child(left_wall)
-- left_wall:set_material(red_mat)
-- left_wall:scale(0.1, 10, 20)
-- left_wall:translate(-6.5, 0, 0)

-- right_wall = gr.cube('right_wall')
-- scene:add_child(right_wall)
-- right_wall:set_material(blue_mat)
-- right_wall:scale(0.1, 10, 20)
-- right_wall:translate(6.5, 0, 0)

-- top_wall = gr.cube('top_wall')
-- scene:add_child(top_wall)
-- top_wall:set_material(green_mat)
-- top_wall:scale(20, 0.1, 20)
-- top_wall:translate(0, 5, 0)

-- bot_wall = gr.cube('bot_wall')
-- scene:add_child(bot_wall)
-- bot_wall:set_material(grey_mat)
-- bot_wall:scale(20, 0.1, 20)
-- bot_wall:translate(0, -5, 0)
    
-- back_wall = gr.cube('back_wall')
-- scene:add_child(back_wall)
-- back_wall:set_material(grey_mat)
-- back_wall:scale(20, 20, 0.1)
-- back_wall:translate(0, 0, -10)

for i = 1, 4 do
    for j = 1, 4 do
        for k = 1, 4 do
            c = gr.nh_box('c' .. tostring(i) .. tostring(j) .. tostring(k), {i - 4, j - 4, k * -4}, 1)
            c:set_material(blue_mat)
            scene:add_child(c)
        end
    end
end


white_light_1 = gr.light({0, 4, -12}, {0.7, 0.7, 0.7}, {1, 0, 0})
-- white_light_1 = gr.light({0, 0, 0}, {0.7, 0.7, 0.7}, {1, 0, 0})
-- white_light_2 = gr.light({0, 0, 15}, {0.5, 0.5, 0.5}, {1, 0, 0})

gr.render(scene, 'results/refraction.png', 256, 256,
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


-- A simple scene demonstrating reflection

mat1 = gr.material({0.3, 0.3, 0.3}, {1.0, 1.0, 1.0}, 25)

scene_root = gr.node('root')

s1 = gr.nh_sphere('s1', {-150, -49, 0}, 150)
scene_root:add_child(s1)
s1:set_material(mat1)

s2 = gr.nh_sphere('s2', {150, -49, 0}, 150)
scene_root:add_child(s2)
s2:set_material(mat1)

white_light = gr.light({-100.0, 150.0, 400.0}, {0.5, 0.5, 0.5}, {1, 0, 0})

gr.render(scene_root, 'refraction.png', 1000, 1000,
    {0, 0, 800}, {0, 0, -1}, {0, 1, 0}, 50,
    {0.3, 0.3, 0.3}, {white_light})

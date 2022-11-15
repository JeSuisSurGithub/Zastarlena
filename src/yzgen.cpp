#include <yzgen.hpp>

namespace yz
{
    i32 lehmer_randrange(u32& state, i32 min, i32 max)
    {
        state = (std::uint64_t)state * 279470273u % 0xfffffffb;
        return (state % (max - min)) + min;
    }

    float lehmer_randrange_flt(u32& state, float min, float max)
    {
        state = (std::uint64_t)state * 279470273u % 0xfffffffb;
        return (float)state / (float)(UINT32_MAX / (max - min)) + min;
    }

    gen_context::gen_context(
        rendergroups::stargroup& stars,
        rendergroups::planetgroup& planets,
        ubo_point_light* point_lights,
        usz& point_light_count,
        u32 seed,
        usz count)
    {
        const float STAR_SPREAD = 5.f * count * count;
        const float BASE_MAX_XZ_COORD = 50000.f;
        const glm::vec3 RGB_BIAS = {10.f, 6.f, 8.f};
        usz planet_count = 0;
        for (point_light_count = 0; point_light_count < count; point_light_count++)
        {
            float scale = lehmer_randrange_flt(seed, 1.f, 1000.f);
            glm::vec3 position = {
                lehmer_randrange_flt(seed, -(BASE_MAX_XZ_COORD / scale) * STAR_SPREAD, (BASE_MAX_XZ_COORD / scale) * STAR_SPREAD),
                0.f,
                lehmer_randrange_flt(seed, -(BASE_MAX_XZ_COORD / scale) * STAR_SPREAD, (BASE_MAX_XZ_COORD / scale) * STAR_SPREAD)};
            glm::vec3 color = {
                lehmer_randrange_flt(seed, 1.f, std::max(RGB_BIAS.r, RGB_BIAS.r * scale / 10.f)),
                lehmer_randrange_flt(seed, 1.f, std::max(RGB_BIAS.g, RGB_BIAS.g * scale / 10.f)),
                lehmer_randrange_flt(seed, 1.f, std::max(RGB_BIAS.b, RGB_BIAS.b * scale / 10.f))};

            add_object(*stars.m_base, "models/uvs1.obj", "textures/unoise.jpg");
            stars.m_base->m_objects[point_light_count].m_translation = position;
            stars.m_base->m_objects[point_light_count].m_scale = glm::vec3(scale, scale, scale);
            point_lights[point_light_count].position = position;
            point_lights[point_light_count].range = rendergroups::light_range_constants(10.f * scale);
            point_lights[point_light_count].color = color;

            m_star_planet_count.push_back(std::clamp<usz>(scale / 10.f, 2.f, 6.f));
            for (usz index = 0; index < m_star_planet_count[point_light_count]; index++, planet_count++)
            {
                float planet_scale = lehmer_randrange_flt(seed, scale / 20.f, scale / 2.f);
                m_planets.push_back(planet_info{
                    .planet_distance_to_star = lehmer_randrange_flt(seed,
                        ((planet_scale / scale) * 80.f + (planet_scale * 40.f)),
                        ((planet_scale / scale) * 700.f + (planet_scale * 40.f))),
                    .planet_revolution_speed = lehmer_randrange_flt(seed,
                        -0.0018f,
                        0.0018f),
                    .planet_orbital_speed = lehmer_randrange_flt(seed,
                        0.0001f,
                        0.0005f),
                    .cur_angle = lehmer_randrange_flt(seed, 0.f, glm::pi<float>())
                });
                glm::vec3 planet_position = {
                    position.x + glm::cos(m_planets[planet_count].cur_angle) * m_planets[planet_count].planet_distance_to_star,
                    0.f,
                    position.z + glm::sin(m_planets[planet_count].cur_angle) * m_planets[planet_count].planet_distance_to_star};
                glm::vec3 planet_color = {
                    lehmer_randrange_flt(seed, 0.f, .5f),
                    lehmer_randrange_flt(seed, 0.f, .5f),
                    lehmer_randrange_flt(seed, 0.f, .5f)};
                add_object(*planets.m_base, "models/uvs1.obj", "textures/venus.png", "textures/cleanpole.png", planet_color);
                planets.m_base->m_objects[planet_count].m_translation = planet_position;
                planets.m_base->m_objects[planet_count].m_scale
                    = glm::vec3(planet_scale, planet_scale, planet_scale);
            }
        }
    }

    gen_context::~gen_context() {}

    void update(
        gen_context& ctx,
        float delta_time,
        rendergroups::stargroup& stars,
        rendergroups::planetgroup& planets)
    {
        usz planet_count = 0;
        for (usz star_count = 0; star_count < stars.m_base->m_objects.size(); star_count++)
        {
            glm::vec3 position = stars.m_base->m_objects[star_count].m_translation;
            for (usz index = 0; index < ctx.m_star_planet_count[star_count]; index++, planet_count++)
            {
                ctx.m_planets[planet_count].cur_angle += delta_time * ctx.m_planets[planet_count].planet_orbital_speed;
                rotate(planets.m_base->m_objects[planet_count],
                    glm::vec3(0.f, delta_time * ctx.m_planets[planet_count].planet_revolution_speed, 0.f));
                planets.m_base->m_objects[planet_count].m_translation = {
                    position.x + glm::cos(ctx.m_planets[planet_count].cur_angle) * ctx.m_planets[planet_count].planet_distance_to_star,
                    0.f,
                    position.z + glm::sin(ctx.m_planets[planet_count].cur_angle) * ctx.m_planets[planet_count].planet_distance_to_star};
            }
        }
    }
}
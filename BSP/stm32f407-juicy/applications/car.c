#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <robot.h>

void car_sample(void)
{
    // 1. Initialize two wheels - left and right
    wheel_t* c_wheels = (wheel_t*) rt_malloc(sizeof(wheel_t) * 2);
    if (c_wheels == RT_NULL)
    {
        LOG_D("Failed to malloc memory for wheels");
    }

    // 1.1 Create two motors
    motor_t left_motor  = motor_create(left_motor_init,  left_motor_enable,  left_motor_disable,  left_motor_set_speed,  DC_MOTOR);
    motor_t right_motor = motor_create(right_motor_init, right_motor_enable, right_motor_disable, right_motor_set_speed, DC_MOTOR);

    // 1.2 Create two encoders
    encoder_t left_encoder  = encoder_create(LEFT_ENCODER_PIN, PULSE_PER_REVOL);
    encoder_t right_encoder = encoder_create(RIGHT_ENCODER_PIN, PULSE_PER_REVOL);

    // 1.3 Create two pid contollers
    pid_control_t left_pid  = pid_create();
    pid_control_t right_pid = pid_create();

    // 1.4 Add two wheels
    c_wheels[0] = wheel_create(left_motor,  left_encoder,  left_pid,  WHEEL_RADIUS, GEAR_RATIO);
    c_wheels[1] = wheel_create(right_motor, right_encoder, right_pid, WHEEL_RADIUS, GEAR_RATIO);

    // 2. Iinialize Kinematics - Two Wheel Differential Drive
    kinematics_t c_kinematics = kinematics_create(TWO_WD, WHEEL_DIST_X, WHEEL_DIST_Y, WHEEL_RADIUS);

    // 3. Initialize Chassis
    chas = chassis_create(c_wheels, c_kinematics);

    // 4. Set Sample time
    encoder_set_sample_time(chas->c_wheels[0]->w_encoder, SAMPLE_TIME);
    encoder_set_sample_time(chas->c_wheels[1]->w_encoder, SAMPLE_TIME);
    pid_set_sample_time(chas->c_wheels[0]->w_pid, PID_SAMPLE_TIME);
    pid_set_sample_time(chas->c_wheels[1]->w_pid, PID_SAMPLE_TIME);

    // 5. Enable Chassis
    chassis_enable(chas);

    // Turn left
    target_vel.linear_x = 0;   // m/s
    target_vel.linear_y = 0;    // m/s
    target_vel.angular_z = PI / 4; // rad/s

    chassis_set_velocity(chas, target_vel);
    rt_thread_mdelay(500);

    // Turn right
    target_vel.linear_x = 0;   // m/s
    target_vel.linear_y = 0;    // m/s
    target_vel.angular_z = - PI / 4; // rad/s

    chassis_set_velocity(chas, target_vel);
    rt_thread_mdelay(500);

    // Go straight
    target_vel.linear_x = 0.06;   // m/s
    target_vel.linear_y = 0;    // m/s
    target_vel.angular_z = 0;

    chassis_set_velocity(chas, target_vel);

    // Stop
    target_vel.linear_x = 0.0;   // m/s
    target_vel.linear_y = 0;    // m/s
    target_vel.angular_z = 0; // rad/s

    chassis_set_velocity(chas, target_vel);
    rt_thread_mdelay(500);

    while(1)
    {
        chassis_update(chas);
        rt_thread_mdelay(50);
    }

}

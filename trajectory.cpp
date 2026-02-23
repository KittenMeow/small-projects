#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

#define SHOOTER_HEIGHT 1.7891 // TODO: Feet
#define HUB_HEIGHT 6.4 // Feet
#define HUB_LENGTH 1.958335 // Feet
#define GRAVITY 32.185 // Feet per second

// Example maximums and minimums
#define MAX_VELOCITY 35
#define MIN_VELOCITY 10
#define MAX_ANGLE (M_PI / 2)
#define MIN_ANGLE (M_PI / 15)

// For optimization
#define STEP 0.1
#define MAX_ITERATIONS 100
#define ERR 0.1

double get_sign(double val){
    if (val > 0){
        return 1.0;
    }
    else if (val < 0){
        return -1.0;
    }
    else {
        return 0;
    }
}

bool bounds(double x, double low, double high){
    if ((x >= low) && (x <= high)){
        return true;
    }
    return false;
}

double trajectory(double distance, double initial_height, double angle, double velocity){
    return (initial_height + (distance * tan(angle)) - ((GRAVITY * distance * distance) / (2 * velocity * velocity * cos(angle) * cos(angle))));
}

double derivative_angle(double distance, double initial_height, double angle, double velocity){
    double h = 0.0000001;
    return (trajectory(distance, initial_height, angle + h, velocity) - trajectory(distance, initial_height, angle - h, velocity)) / (2 * h);
}

double derivative_velocity(double distance, double initial_height, double angle, double velocity){
    double h = 0.0000001;
    return (trajectory(distance, initial_height, angle, velocity + h) - trajectory(distance, initial_height, angle, velocity - h)) / (2 * h);
}

double* optimize(double distance, double initial_height, double angle, double velocity){
    double *step = new double[2];
    double new_angle = angle;
    double new_velocity = velocity;
    double distance_diff;
    double height_low, height_high;

    while (true){
        // Get the height_low (before the hub) and height_high (after the hub)
        height_low = trajectory(distance - HUB_LENGTH, initial_height, new_angle, new_velocity);
        height_high = trajectory(distance + HUB_LENGTH, initial_height, new_angle, new_velocity);
        cout << "Angle: " << new_angle << "   Velocity: " << new_velocity << "   Height low: " << height_low << "   Height high: " << height_high << endl;
        if (height_low > HUB_HEIGHT && height_high < HUB_HEIGHT){
            step[0] = new_angle;
            step[1] = new_velocity;
            break;
        }

        // Change the velocity based on the derivative (normalized)
        new_velocity += get_sign(derivative_velocity(distance, initial_height, new_angle, new_velocity)) * STEP;
        if ((new_velocity > MAX_VELOCITY)){
            // If the velocity gets too big, reset it and step the angle up.
            new_angle += get_sign(derivative_angle(distance, initial_height, new_angle, new_velocity)) * STEP;
            new_velocity = MIN_VELOCITY;
        }
    }

    return step;
}

int main(){
    double robot_velocity = 0; // ft/s
    double hub_distance = 25; // feet
    double angle = MIN_ANGLE; // radians, example starting
    double velocity = MIN_VELOCITY; // ft/s, output
    //double *result = new double[2];

    // TODO: Faster optimizer

    // TODO: Implement robot velocity into the velocity of the flywheel

    //cout << trajectory(hub_distance, SHOOTER_HEIGHT, M_PI / 4, 10) << endl;
    //cout << "Angle: " << angle << endl;
    //cout << "Velocity: " << vel << endl;
    //cout << "Distance at zero: " << traj_zero(SHOOTER_HEIGHT, angle, vel) << endl;

    double* result = optimize(hub_distance, SHOOTER_HEIGHT, angle, velocity);
    //double height = trajectory(distance_diff, initial_height, result[0], result[1]);
    if ((result[0] < MIN_ANGLE) || (result[0] > MAX_ANGLE)){
        cout << "Angle not found." << endl;
    }
    if ((result[1] < MIN_VELOCITY) || (result[1] > MAX_VELOCITY)){
        cout << "Velocity not found." << endl;
    }

    cout << "Angle: " << result[0] << endl;
    cout << "Velocity: " << result[1] << endl;

    return 0;
}

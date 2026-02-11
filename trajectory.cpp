#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

#define SHOOTER_HEIGHT 0.5 // TODO: Feet per second
#define HUB_HEIGHT 6 // Feet
#define GRAVITY 32.185 // Feet per second

// Example maximums and minimums
#define MAX_VELOCITY 33 
#define MIN_VELOCITY 20
#define MAX_ANGLE (M_PI / 2)
#define MIN_ANGLE 0

// For optimization
#define ERROR_OFFSET 0.01
#define MAX_ITERATIONS 200 // If it takes too long, stop it.

double trajectory(double distance, double initial_height, double angle, double velocity){
    return initial_height + distance * tan(angle) - ((GRAVITY * distance * distance) / (2 * velocity * velocity * cos(angle) * cos(angle)));
}

double trajectory_derivative_angle(double distance, double initial_height, double angle, double velocity){
    return (distance * sec(angle) * sec(angle)) - ((GRAVITY * 4 * sec(angle) * sec(angle) * tan(angle)) / (velocity * velocity));
}

double trajectory_derivative_velocity(double distance, double initial_height, double angle, double velocity){
    return (-GRAVITY * distance * distance) / (-velocity * velocity * velocity * cos(angle) * cos(angle));
}

double guess_velocity(double distance, double angle){
    return (2 * GRAVITY * sqrt(tan(angle))) / (sqrt(distance));
}

double guess_angle(double distance, double velocity){
    return atan((distance * velocity * velocity) / (4 * GRAVITY * GRAVITY));
}

vector<double> optimize(double distance, double initial_height, double angle){
    double velocity;
    int iterations = 0;
    while (true){
        velocity = guess_velocity(distance, angle);
        vector<double> result = {angle, velocity};
        if (iterations > MAX_ITERATIONS){
            break;
        }
        double traj = trajectory(distance, initial_height, result[0], result[1]);
        if ((velocity <= MAX_VELOCITY) && (velocity >= MIN_VELOCITY) && (traj > 6)){
            result = {angle, velocity};
            return result;
        }
        else if ((velocity < MAX_VELOCITY) || (traj <= 6)) {
            angle = guess_angle(distance, velocity + ERROR_OFFSET);
            iterations++;
            continue;
        }
        else {
            angle = guess_angle(distance, velocity - ERROR_OFFSET);
            iterations++;
            continue;
        }
    }

    vector<double> result = {-1, -1};
    return result;
}

int main(){
    double robot_velocity = 0; // ft/s
    double hub_distance = 12.5; // feet
    double angle = M_PI / 3; // radians, example starting
    double flywheel_velocity = 9; // ft/s, output

    // TODO: Optimizer will sometimes not find an answer when there is one
    // Solution: Use an actual optimizer rather than the silliness I wrote

    // TODO: Implement robot velocity into the velocity of the flywheel
    
    vector<double> result = optimize(hub_distance, SHOOTER_HEIGHT, angle);
    cout << "Angle: " << result[0] << "   Velocity: " << result[1] << endl;
    double traj = trajectory(hub_distance, SHOOTER_HEIGHT, result[0], result[1]);
    cout << "Final Height: " << traj << endl;
}
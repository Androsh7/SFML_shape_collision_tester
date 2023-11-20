#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <algorithm>



float max(float a, float b) {
    if (a >= b) {
        return a;
    }
        return b;
}

float min(float a, float b) {
    if (a <= b) {
        return a;
    }
        return b;
}


struct line {
    sf::Vector2f p1, p2;
};

sf::Vector2f minArray (std::vector<line> inputvector) {
    sf::Vector2f minPoint = inputvector[0].p1;
    for (int i = 0; i < inputvector.size(); i++) {
        if (inputvector[i].p1.x > minPoint.x) {
            minPoint.x = inputvector[i].p1.x;
        }
        if (inputvector[i].p2.x > minPoint.x) {
            minPoint.x = inputvector[i].p2.x;
        }
        if (inputvector[i].p1.y > minPoint.y) {
            minPoint.y = inputvector[i].p1.y;
        }
        if (inputvector[i].p2.y > minPoint.y) {
            minPoint.y = inputvector[i].p2.y;
        }
    }
    return minPoint;
}

sf::Vector2f maxArray(std::vector<line> inputvector) {
    sf::Vector2f maxPoint = inputvector[0].p1;
    int i = 0;
    for (std::vector<line>::iterator iter = inputvector.begin(); iter != inputvector.end(); ++iter) {
        if (inputvector[i].p1.x < maxPoint.x) {
            maxPoint.x = inputvector[i].p1.x;
        }
        if (inputvector[i].p2.x < maxPoint.x) {
            maxPoint.x = inputvector[i].p2.x;
        }
        if (inputvector[i].p1.y < maxPoint.y) {
            maxPoint.y = inputvector[i].p1.y;
        }
        if (inputvector[i].p2.x < maxPoint.y) {
            maxPoint.y = inputvector[i].p2.y;
        }
        i++;
    }
    return maxPoint;
}

bool onSegment(sf::Vector2f p, sf::Vector2f q, sf::Vector2f r) {
    return (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
        q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y));
}

int orientation(sf::Vector2f p, sf::Vector2f q, sf::Vector2f r) {
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    return (val == 0) ? 0 : (val > 0) ? 1 : 2;
}

bool doIntersect(line line1, line line2) {
    int orientation1 = orientation(line1.p1, line1.p2, line2.p1);
    int orientation2 = orientation(line1.p1, line1.p2, line2.p2);
    int orientation3 = orientation(line2.p1, line2.p2, line1.p1);
    int orientation4 = orientation(line2.p1, line2.p2, line1.p2);

    if ((orientation1 != orientation2) && (orientation3 != orientation4)) {
        return true; // The lines intersect
    }

    if (orientation1 == 0 && onSegment(line1.p1, line2.p1, line1.p2) ||
        orientation2 == 0 && onSegment(line1.p1, line2.p2, line1.p2) ||
        orientation3 == 0 && onSegment(line2.p1, line1.p1, line2.p2) ||
        orientation4 == 0 && onSegment(line2.p1, line1.p2, line2.p2)) {
        return true; // The lines are collinear and overlap
    }

    return false; // The lines do not intersect
}

bool polyCollisionCheck(std::vector<line> boundary1, std::vector<line> boundary2) {
    for (int bound1 = 0; bound1 < boundary1.size(); bound1++) {
        for (int bound2 = 0; bound2 < boundary2.size(); bound2++) {
            if (doIntersect(boundary1[bound1], boundary2[bound2])) {
                //std::cout << "------------\n";
                //std::cout << bound1 << "," << bound2 << "(" << boundary1[bound1].p1.x << "," << boundary1[bound1].p1.y << ")" << " to " << "(" << boundary1[bound1].p2.x << "," << boundary1[bound1].p2.y << ")\n";
                //std::cout << bound1 << "," << bound2 << "(" << boundary2[bound2].p1.x << "," << boundary2[bound2].p1.y << ")" << " to " << "(" << boundary2[bound2].p2.x << "," << boundary2[bound2].p2.y << ")\n";
                return true;
            }
        }
    }
    return false;
}

class hitbox {
public:
    
    sf::ConvexShape polygon;
    std::vector<line> boundary;
    sf::Vector2f minPoint = sf::Vector2f(0, 0);
    sf::Vector2f maxPoint = sf::Vector2f(1, 1);
    int pointTotal = polygon.getPointCount();
    
    void printBounds() {
        for (int i = 0; i < boundary.size(); i++) {
            std::cout << "i = "<< i<< " " << "(" << boundary[i].p1.x << "," << boundary[i].p1.y << ")" << " to " << "(" << boundary[i].p2.x << "," << boundary[i].p2.y << "\n";
        }
        std::cout << "min Point: " << "(" << minPoint.x << "," << minPoint.y << ")\n";
        std::cout << "max Point: " << "(" << maxPoint.x << "," << maxPoint.y << ")\n";
    }
    
    // updates the boundary vector
    void update_poly() {
        
        // clears the boundary vector
        boundary.resize(0);

        // sizes the boundary array to the polygon
        boundary.resize(polygon.getPointCount());

        // updates the points in the boundary vector    
        for (int i = 0; i < (polygon.getPointCount()); i++) {
            boundary[i].p1.x = polygon.getPoint(i).x + polygon.getPosition().x;
            boundary[i].p1.y = polygon.getPoint(i).y + polygon.getPosition().y;
            boundary[i].p2.x = polygon.getPoint(i + 1).x + polygon.getPosition().x;
            boundary[i].p2.y = polygon.getPoint(i + 1).y + polygon.getPosition().y;;
        }
        boundary[polygon.getPointCount() - 1].p1.x = polygon.getPoint(polygon.getPointCount() - 1).x + polygon.getPosition().x;
        boundary[polygon.getPointCount() - 1].p1.y = polygon.getPoint(polygon.getPointCount() - 1).y + polygon.getPosition().y;
        boundary[polygon.getPointCount() - 1].p2.x = polygon.getPoint(0).x + polygon.getPosition().x;
        boundary[polygon.getPointCount() - 1].p2.y = polygon.getPoint(0).y + polygon.getPosition().y;

        // updates the minpoint and maxpoint
        minPoint = minArray(boundary);
        maxPoint = maxArray(boundary);
    }

    void move(float x, float y, std::vector<line> hitbox) {
        sf::Vector2f newPos = sf::Vector2f(polygon.getPosition().x + x, polygon.getPosition().y + y);
        
        if (newPos.x > 500) {
            newPos.x = 500;
        }
        else if (newPos.x < 0) {
            x = 0;
            newPos.x = 0;
        }
        if (newPos.y > 500) {
            y = 0;
            newPos.y = 500;
        }
        else if (newPos.y < 0) {
            y = 0;
            newPos.y = 0;
        }
        polygon.setPosition(newPos);

        update_poly();
        //printBounds();
    }
};



int main() {
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

    hitbox player;
    player.polygon.setFillColor(sf::Color::White);
    player.polygon.setPointCount(3);
    player.polygon.setPoint(0, sf::Vector2f(0, 0));
    player.polygon.setPoint(1, sf::Vector2f(100, 100));
    player.polygon.setPoint(2, sf::Vector2f(200, 0));
    player.polygon.setPosition(sf::Vector2f(0.1, 0.1));

    hitbox obstacle;
    obstacle.polygon.setFillColor(sf::Color::Red);
    obstacle.polygon.setPointCount(4);
    obstacle.polygon.setPoint(0, sf::Vector2f(0, 0));
    obstacle.polygon.setPoint(1, sf::Vector2f(50, 0));
    obstacle.polygon.setPoint(2, sf::Vector2f(50, 50));
    obstacle.polygon.setPoint(2, sf::Vector2f(0, 50));
    obstacle.polygon.setPosition(sf::Vector2f(150, 150));

    hitbox goal;
    goal.polygon.setFillColor(sf::Color::Green);
    goal.polygon.setPointCount(4);
    goal.polygon.setPoint(0, sf::Vector2f(0, 0));
    goal.polygon.setPoint(1, sf::Vector2f(50, 0));
    goal.polygon.setPoint(2, sf::Vector2f(50, 50));
    goal.polygon.setPoint(2, sf::Vector2f(0, 50));
    goal.polygon.setPosition(sf::Vector2f(400, 400));

    sf::CircleShape collision(50);
    collision.setFillColor(sf::Color::Cyan);
    collision.setPosition(sf::Vector2f(500, 0));

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Clear screen
        window.clear();

        // draws the player and obstacle
        window.draw(player.polygon);
        if (polyCollisionCheck(player.boundary, obstacle.boundary)) {
            window.draw(collision);
        }
        window.draw(obstacle.polygon);
        window.draw(goal.polygon);

        // Update the window
        window.display();

        
        // player input loop
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            player.move(-0.1, 0,obstacle.boundary);
            //std::cout << "left key pressed\n";
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            player.move(0.1, 0, obstacle.boundary);
            //std::cout << "right key pressed\n";
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            player.move(0, -0.1, obstacle.boundary);
            //std::cout << "up key pressed\n";
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            player.move(0, 0.1, obstacle.boundary);
            //std::cout << "down key pressed\n";
        }
        
        if (polyCollisionCheck(player.boundary, obstacle.boundary)) {
            player.polygon.setPosition(sf::Vector2f(0, 0));
            std::cout << "PLAYER HIT OBSTACLE\n";
        }
        if (polyCollisionCheck(player.boundary, goal.boundary)) {
            player.polygon.setPosition(sf::Vector2f(0, 0));
            std::cout << "GOAL REACHED\n";
        }

        //std::cout << obstacle.polygon.getPosition().x << "," << obstacle.polygon.getPosition().y << "\n";
        obstacle.update_poly();
        goal.update_poly();

    }

    return EXIT_SUCCESS;
}

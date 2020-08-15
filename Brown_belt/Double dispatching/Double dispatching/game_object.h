//
//  game_object.h
//  Double dispatching
//
//  Created by Andrew Kireev on 15.08.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#ifndef game_object_h
#define game_object_h

class Unit;
class Building;
class Tower;
class Fence;

struct GameObject {
  virtual ~GameObject() = default;

  virtual bool Collide(const GameObject& that) const = 0;
  virtual bool CollideWith(const Unit& that) const = 0;
  virtual bool CollideWith(const Building& that) const = 0;
  virtual bool CollideWith(const Tower& that) const = 0;
  virtual bool CollideWith(const Fence& that) const = 0;
};

bool Collide(const GameObject& first, const GameObject& second);

#endif /* game_object_h */

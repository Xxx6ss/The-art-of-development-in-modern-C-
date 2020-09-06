//
//  Solution.cpp
//  Texture
//
//  Created by Andrew Kireev on 04.09.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#include "Common.h"
#include <memory>

using namespace std;

// Этот файл сдаётся на проверку
// Здесь напишите реализацию необходимых классов-потомков `IShape`


class Figure : public IShape {
public:
//    Figure() = default;
    
    Figure(Point position, Size size, ShapeType type) : position_(position),
                                size_(size), type_(type) {}
    
    
    Figure(ShapeType type) : type_(type) {}
    
    void SetPosition(Point position) override {
        position_ = move(position);
    }
    
    Point GetPosition() const override {
        return position_;
    }
    
    void SetSize(Size size) override {
        size_ = size;
    }
    
    Size GetSize() const override {
        return size_;
    }
    
    std::unique_ptr<IShape> Clone() const override {
        return make_unique<Figure>(Figure(position_, size_, type_));
        
    }
    
    void SetTexture(std::shared_ptr<ITexture> texture) override {
        texture_ = move(texture);
    }
    
    ITexture* GetTexture() const override {
        return texture_.get();
    }
    
    void Draw(Image& image) const override {
        for (int i = 0; i < size_.height; ++i) {
            for (int j = 0; j < size_.width; ++j) {
                if (type_ == ShapeType::Ellipse && !IsPointInEllipse(Point{ j, i}, size_))
                    continue;
                int x = j + position_.x;
                int y = i + position_.y;
                if (!(y < image.size() && x < image[y].size()))
                    continue;
                if (texture_ && i < texture_->GetSize().height && j < texture_->GetSize().width)
                    image[y][x] = texture_->GetImage()[i][j];
                else
                    image[y][x] = '.';
            }
        }
    }
    
private:
    Point position_;
    Size size_;
    std::shared_ptr<ITexture> texture_;
    ShapeType type_;
};




// Напишите реализацию функции
unique_ptr<IShape> MakeShape(ShapeType shape_type) {
    return make_unique<Figure>(shape_type);
}

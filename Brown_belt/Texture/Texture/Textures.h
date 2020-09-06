//
//  Textures.h
//  Texture
//
//  Created by Andrew Kireev on 04.09.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#ifndef Textures_h
#define Textures_h

#include "Common.h"

#include <memory>

std::unique_ptr<ITexture> MakeTextureSolid(Size size, char pixel);

std::unique_ptr<ITexture> MakeTextureCheckers(Size size, char pixel1,
                                              char pixel2);

std::unique_ptr<ITexture> MakeTextureCow();

#endif /* Textures_h */

//
//  SWPlayer.h
//  SteamedWorlds
//
//  Copyright 2014 by Überpixel. All rights reserved.
//  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
//  documentation files (the "Software"), to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
//  and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
//  PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
//  FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#ifndef __SWPlayer__
#define __SWPlayer__

#include <Rayne/Rayne.h>
#include <RBKinematicController.h>

namespace SW
{
	class Player : public RN::Entity, public RN::INonConstructingSingleton<Player>
	{
	public:
		Player(RN::SceneNode *camera);
		~Player() override;
		
		void Update(float delta) override;
		void SetPassable(bool passable);
		
	private:
		RN::SceneNode *_camera;
		RN::bullet::KinematicController *_controller;
		
		bool _isPassable;
		
		RNDeclareMeta(Player)
		RNDeclareSingleton(Player)
	};
}

#endif /* defined(__SWPlayer__) */

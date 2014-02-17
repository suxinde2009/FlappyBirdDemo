/*
 The MIT License (MIT)
 
 Copyright (c) 2013 SuXinDe (Email: suxinde2009@126.com)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

#include "HelloWorldScene.h"

USING_NS_CC;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    ////////////////////////////////////////////////////
    // 下面添加创建自己的Sprite的代码
    ////////////////////////////////////////////////////
    
    mScreenSize = CCDirector::sharedDirector()->getVisibleSize();
    initWorld();
    addBird();
    addBarContainer();
    
    setTouchEnabled(true);
    
    scheduleOnce(schedule_selector(HelloWorld::startGame), 3);
    
    return true;
}

void HelloWorld::startGame(float dt){
    scheduleUpdate();
    schedule(schedule_selector(HelloWorld::addBar), 1);
}

void HelloWorld::stopGame(){
    unscheduleUpdate();
    unschedule(schedule_selector(HelloWorld::addBar));
}

void HelloWorld::initWorld(){
    mWorld = new b2World(b2Vec2(0, -10));
    mWorld->SetContactListener(this);
}

void HelloWorld::addBird(){
    mBird = B2Sprite::create("bird.png");
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    // 单位为 米
    bodyDef.position = b2Vec2(mScreenSize.width/2.0f/RATIO, 9); // 10m以内模拟效果较高效。
    b2Body *birdBody = mWorld->CreateBody(&bodyDef);
    
    // 碰撞
    CCSize birdSize = mBird->getContentSize();
    b2PolygonShape birdShape;
    birdShape.SetAsBox(birdSize.width/2.0f/RATIO, birdSize.height/2.0f/RATIO); // 半宽，半高
    b2FixtureDef birdFixtureDef;
    birdFixtureDef.shape = &birdShape;
    
    birdBody->CreateFixture(&birdFixtureDef);
    
    mBird->setPTMRatio(RATIO);
    mBird->setB2Body(birdBody);
    addChild(mBird);
}

void HelloWorld::addGround() {
    B2Sprite *ground = B2Sprite::create("ground.png");
    CCSize groundSize = ground->getContentSize();
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position = b2Vec2(groundSize.width/2.0f/RATIO,
                              groundSize.height/2.0f/RATIO);
    b2Body *groundBody = mWorld->CreateBody(&bodyDef);

    b2PolygonShape groundShape;
    groundShape.SetAsBox(groundSize.width/2.0f/RATIO, groundSize.height/2.0f/RATIO);
    b2FixtureDef groundFixtureDef;
    groundFixtureDef.shape = &groundShape;
    groundBody->CreateFixture(&groundFixtureDef);
    
    ground->setB2Body(groundBody);
    ground->setPTMRatio(RATIO);
    addChild(ground);
    
}

void HelloWorld::addBarContainer() {
    mBarContainer = CCSprite::create();
    addChild(mBarContainer);
}

void HelloWorld::addBar(float dt){
    float offset = -rand()%5;
    
    // 下面的柱子
    B2Sprite *downBar = B2Sprite::create("down_bar.png");
    CCSize downBarSize = downBar->getContentSize();
    
    b2BodyDef downBarBodyDef;
    downBarBodyDef.type = b2_kinematicBody;
    downBarBodyDef.position = b2Vec2(mScreenSize.width/RATIO+2,
                                     downBarSize.height/RATIO/2+offset);
    downBarBodyDef.linearVelocity = b2Vec2(-5, 0);
    b2Body *downBarBody = mWorld->CreateBody(&downBarBodyDef);
    
    b2PolygonShape downBarShape;
    downBarShape.SetAsBox(downBarSize.width/2/RATIO,
                          downBarSize.height/2/RATIO);
    b2FixtureDef downBarFixtureDef;
    downBarFixtureDef.shape = &downBarShape;
    downBarBody->CreateFixture(&downBarFixtureDef);
    
    downBar->setB2Body(downBarBody);
    downBar->setPTMRatio(RATIO);
    mBarContainer-> addChild(downBar);

    
    //上面的柱子
    B2Sprite *upBar = B2Sprite::create("up_bar.png");
    CCSize upBarSize = upBar->getContentSize();
    
    b2BodyDef upBarBodyDef;
    upBarBodyDef.type = b2_kinematicBody;
    upBarBodyDef.position = b2Vec2(mScreenSize.width/RATIO+2,
                                   downBarSize.height/RATIO+offset+2+upBarSize.height/2/RATIO);
    upBarBodyDef.linearVelocity = b2Vec2(-5, 0);
    b2Body *upBarDody = mWorld->CreateBody(&upBarBodyDef);
    
    b2PolygonShape upBarShape;
    upBarShape.SetAsBox(upBarSize.width/2/RATIO, upBarSize.height/2/RATIO);
    b2FixtureDef upBarFixtureDef;
    upBarFixtureDef.shape = &upBarShape;
    upBarDody->CreateFixture(&upBarFixtureDef);
    
    upBar->setB2Body(upBarDody);
    upBar->setPTMRatio(RATIO);
    mBarContainer-> addChild(upBar);
}

void HelloWorld::update(float dt){
    mWorld->Step(dt, 8, 3); // 8和3为官方推荐数据
    
    CCSprite *s;
    
    for(b2Body *b = mWorld->GetBodyList(); b != NULL; b = b->GetNext()){
        if(b->GetPosition().x < -3){
            s = (CCSprite *)b->GetUserData();
            if(s != NULL){
                s->removeFromParent();
            }
            mWorld->DestroyBody(b);
        }
    }
}

void HelloWorld::BeginContact(b2Contact *contact){
    if(contact->GetFixtureA()->GetBody()->GetUserData() == mBird ||
       contact->GetFixtureB()->GetBody()->GetUserData() == mBird){
        stopGame();
        CCMessageBox("Game Over!", "Game Over!");
    }
}

void HelloWorld::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
    mBird->getB2Body()->SetLinearVelocity(b2Vec2(0.0f, 5.0f));
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

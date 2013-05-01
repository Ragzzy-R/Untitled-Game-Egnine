/*=======================================================================
 * Author      : Raghuram Iyer Razzy-R									
 * Description : this is an interface for creating your states.override
				 these virtual functions in order to render, update and
				 init
 * created on  : 24/04/2013                                             
 * Last Edited : 27/04/2013                                             
 =======================================================================*/

#pragma once
class AppGameState;
class GameState
{
public:
	GameState(void);
	virtual void init(AppGameState* apg){
		m_apg = apg;
	}

	virtual void update()=0;
	virtual void render()=0;
	void pause(bool);
	~GameState(void);
	
protected:
	AppGameState* m_apg;
};
	

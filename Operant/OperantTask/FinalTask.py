#Dependencies
from pyControl.utility import *
from machine import Pin
from micropython import const

#Hardware definition - in hardware file
import hardware_definition as hw
#Define task
states = ['wait_for_poke',
          'reward',
          'timeout',
          'continue_reward']

events = ['left_poke', 
          'right_poke',
          'left_poke_out', 
          'right_poke_out',
          'session_timer',
          'left_reward_timer',
          'right_reward_timer',
          'MPR_down',
          'next_reward',
          'RightLick',
          'LeftLick']

initial_state = 'wait_for_poke'

#Rename pumps for convenience
leftpump = hw.left_poke.SOL
rightpump= hw.right_poke.SOL
leftsol = hw.left_poke.PUFF
rightsol = hw.right_poke.PUFF

#Initialise Constants
v.count = 0
v.session_duration = ((4 * 24) +16 )* hour 
v.reward_duration = 250 * ms  
v.left_rewards_obtained = 0
v.right_rewards_obtained = 0
v.current_poke = ''
v.left_reward_limit = 0
v.right_reward_limit = 0
v.proceed = False
v.lockout_duration = 30000 * ms
v.reward_wait= 0 * ms
v.max_reward = 200
v.continue_reward_time=0.1*second

#housekeeping
def run_start():
    hw.right_poke.SOL.off()
    hw.left_poke.SOL.off()
    hw.right_poke.PUFF.off()
    hw.left_poke.PUFF.off()

def run_end():
    hw.off()
    hw.lights.fill((0,0,0))

#statemachine
def wait_for_poke(event):
    if event == 'entry':
        hw.lights.fill((0,0,0))
        hw.lights[hw.LeftPanelRGB]=(10,10,10) 
        hw.lights[hw.RightPanelRGB]=(10,10,10) 
      
    if event == 'left_poke' or event == 'right_poke':
        v.proceed = False
        if event == 'left_poke':
            v.current_poke='left'
            if v.left_reward_limit < v.max_reward:
                v.proceed = True
                hw.lights.fill((0,0,0))
                hw.lights[hw.LeftPokeRGB]=(10,10,10) 
        elif event == 'right_poke':
            v.current_poke='right'
            if v.right_reward_limit < v.max_reward:
                v.proceed = True
                hw.lights.fill((0,0,0))
                hw.lights[hw.RightPokeRGB]=(10,10,10)  
        if v.proceed == True:        
            timed_goto_state('reward', v.reward_wait)
        elif v.proceed == False:
            print("Timed out; right reward limit = " + str(v.right_reward_limit) + " Left reward limit = "+ str(v.left_reward_limit))
            if v.current_poke== 'left':
                pass
                hw.lights[hw.LeftPokeRGB]=(0,0,0)      
                hw.lights[hw.LeftPokeRGB]=(0,0,0)      
            elif v.current_poke=='right':
                pass
                hw.lights[hw.RightPokeRGB]=(0,0,0)      
                hw.lights[hw.RightPokeRGB]=(0,0,0)      
            timed_goto_state('wait_for_poke', v.lockout_duration)
            
def continue_reward(event):
    if event =='next_reward':
        print("next reward triggered")
        v.proceed = False
        if not v.current_poke== None:
            print("Running")
            if v.current_poke== 'left':
                if v.left_reward_limit < v.max_reward:
                    v.proceed = True
            elif v.current_poke=='right':
                if v.right_reward_limit < v.max_reward:
                    v.proceed = True
            if v.proceed == True:        
                print("Proceedable")
                timed_goto_state('reward',v.reward_duration/2)
            elif v.proceed == False:
                print("Timed out; right reward limit = " + str(v.right_reward_limit) + " Left reward limit = "+ str(v.left_reward_limit))
                hw.lights.fill((0,0,0))
                timed_goto_state('wait_for_poke', v.lockout_duration) 
        else:
            goto_state('wait_for_poke')

def reward(event):
    if event == 'entry':
        timed_goto_state('continue_reward', v.reward_duration/2)
        if v.current_poke == 'left':
            leftpump.on()
            v.left_rewards_obtained += 1
            v.left_reward_limit += 1
            set_timer('left_reward_timer', 120*second, output_event=True)  
        elif v.current_poke == 'right':
            rightpump.on()
            v.right_rewards_obtained += 1
            v.right_reward_limit += 1            
            set_timer('right_reward_timer', 120*second, output_event=True)
        print('Left Rewards obtained: {}'.format(v.left_rewards_obtained))
        print('Right Rewards obtained: {}'.format(v.right_rewards_obtained))
    elif event == 'exit':
        hw.right_poke.SOL.off()
        hw.left_poke.SOL.off()
        hw.right_poke.PUFF.off()
        hw.left_poke.PUFF.off()
        set_timer('next_reward', v.continue_reward_time, output_event=True)

# State independent behaviour.

def all_states(event):
    # When 'session_timer' event occurs stop framework to end session.
    if event == 'session_timer':
        stop_framework()
    if event == 'left_reward_timer':
        v.left_reward_limit = v.left_reward_limit-1
    if event == 'right_reward_timer':
        v.right_reward_limit = v.right_reward_limit -1
    if event =='MPR_down':
        if hw.touchsensor.is_touched(0):
                print('Left Lick detected!')
                publish_event('LeftLick')
        elif hw.touchsensor.is_touched(1):
                print('Right Lick detected!')
                publish_event('RightLick')
    if event == 'left_poke_out' and v.current_poke=='left':
        v.current_poke=None
        print("pokereset")
    if event == 'right_poke_out' and v.current_poke=='right':
        v.current_poke=None
        print("pokereset")

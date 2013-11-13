--脚本号
x414009_g_scriptId = 414009 --临时写这个,真正用的时候一定要改.
x414009_g_keyID = 12100009 --临时写这个,真正用的时候一定要改.
x414009_g_keyname = "钥匙（9级）" --临时写这个,真正用的时候一定要改.


--需要的等级

--**********************************
--事件交互入口
--**********************************
function x414009_OnDefaultEvent( sceneId, selfId, bagIndex )
	local HaveKey = HaveItemInBag(sceneId,selfId,x414009_g_keyID)
	
	if HaveKey > 0 then
		OpenRndomBox ( sceneId, selfId, 1, 2, 5, 3, 5, 4, 5, 5, 3, 6, 2) 
		DelItem(sceneId,selfId,x414009_g_keyID,1)
		LuaFnEraseItem(sceneId,selfId,bagIndex)
		BeginEvent(sceneId)
			strText = "#Y你使用一把"..x414009_g_keyname.."，开启了一个宝箱"
			AddText(sceneId,strText);
		EndEvent(sceneId)
		DispatchMissionTips(sceneId,selfId)
	else
		BeginEvent(sceneId)
			strText = "#Y开启宝箱，需要"..x414009_g_keyname.."一把"
			AddText(sceneId,strText);
		EndEvent(sceneId)
		DispatchMissionTips(sceneId,selfId)
	end
end

--**********************************
--这个物品的使用过程是否类似于技能：
--系统会在执行开始时检测这个函数的返回值，如果返回失败则忽略后面的类似技能的执行。
--返回1：技能类似的物品，可以继续类似技能的执行；返回0：忽略后面的操作。
--**********************************
function x414009_IsSkillLikeScript( sceneId, selfId)
	return 0; --这个脚本需要动作支持
end

--功能：学习采矿技能

x510001_g_ScriptId = 510001
x510001_g_MissionName="学习采矿技能"
--**********************************

--任务入口函数

--**********************************

function x510001_OnDefaultEvent(sceneId, selfId, targetId)	--点击该任务后执行此脚本
--	DismantleSouXiaItem( sceneId, selfId, targetId)
end



--**********************************

--列举事件

--**********************************

function x510001_OnEnumerate(sceneId, selfId, targetId)
		AddNumText(sceneId, x510001_g_ScriptId, x510001_g_MissionName)
end



--**********************************

--检测接受条件

--**********************************

function x510001_CheckAccept(sceneId, selfId, targetId)

end

--**********************************

--接受

--**********************************

function x510001_OnAccept(sceneId, selfId)
                                                                   
	     
end



--**********************************

--放弃

--**********************************

function x510001_OnAbandon(sceneId, selfId)

end



--**********************************

--检测是否可以提交

--**********************************

function x510001_CheckSubmit( sceneId, selfId, targetId)

end



--**********************************

--提交

--**********************************

function x510001_OnSubmit(sceneId, selfId, targetId, selectRadioId)
	
end



--**********************************

--杀死怪物或玩家

--**********************************

function x510001_OnKillObject(sceneId, selfId, objdataId)

end



--**********************************

--进入区域事件

--**********************************

function x510001_OnEnterArea(sceneId, selfId, zoneId)

end



--**********************************

--道具改变

--**********************************

function x510001_OnItemChanged(sceneId, selfId, itemdataId)

end

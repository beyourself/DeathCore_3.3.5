/*
 * Copyright (C) 2013-2015 DeathCore <http://www.noffearrdeathproject.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TRINITY_SMARTSCRIPT_H
#define TRINITY_SMARTSCRIPT_H

#include "Common.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "Unit.h"
#include "Spell.h"
#include "GridNotifiers.h"

#include "SmartScriptMgr.h"
//#include "SmartAI.h"

class SmartScript
{
    public:
        SmartScript();
        ~SmartScript();

        void OnInitialize(WorldObject* obj, AreaTriggerEntry const* at = NULL);
        void GetScript();
        void FillScript(SmartAIEventList e, WorldObject* obj, AreaTriggerEntry const* at);

        void ProcessEventsFor(SMART_EVENT e, Unit* unit = NULL, uint32 var0 = 0, uint32 var1 = 0, bool bvar = false, const SpellInfo* spell = NULL, GameObject* gob = NULL);
        void ProcessEvent(SmartScriptHolder& e, Unit* unit = NULL, uint32 var0 = 0, uint32 var1 = 0, bool bvar = false, const SpellInfo* spell = NULL, GameObject* gob = NULL);
        bool CheckTimer(SmartScriptHolder const& e) const;
        void RecalcTimer(SmartScriptHolder& e, uint32 min, uint32 max);
        void UpdateTimer(SmartScriptHolder& e, uint32 const diff);
        void InitTimer(SmartScriptHolder& e);
        void ProcessAction(SmartScriptHolder& e, Unit* unit = NULL, uint32 var0 = 0, uint32 var1 = 0, bool bvar = false, const SpellInfo* spell = NULL, GameObject* gob = NULL);
        void ProcessTimedAction(SmartScriptHolder& e, uint32 const& min, uint32 const& max, Unit* unit = NULL, uint32 var0 = 0, uint32 var1 = 0, bool bvar = false, const SpellInfo* spell = NULL, GameObject* gob = NULL);
        ObjectList* GetTargets(SmartScriptHolder const& e, Unit* invoker = NULL);
        ObjectList* GetWorldObjectsInDist(float dist);
        void InstallTemplate(SmartScriptHolder const& e);
        SmartScriptHolder CreateEvent(SMART_EVENT e, uint32 event_flags, uint32 event_param1, uint32 event_param2, uint32 event_param3, uint32 event_param4, SMART_ACTION action, uint32 action_param1, uint32 action_param2, uint32 action_param3, uint32 action_param4, uint32 action_param5, uint32 action_param6, SMARTAI_TARGETS t, uint32 target_param1, uint32 target_param2, uint32 target_param3, uint32 phaseMask = 0);
        void AddEvent(SMART_EVENT e, uint32 event_flags, uint32 event_param1, uint32 event_param2, uint32 event_param3, uint32 event_param4, SMART_ACTION action, uint32 action_param1, uint32 action_param2, uint32 action_param3, uint32 action_param4, uint32 action_param5, uint32 action_param6, SMARTAI_TARGETS t, uint32 target_param1, uint32 target_param2, uint32 target_param3, uint32 phaseMask = 0);
        void SetPathId(uint32 id) { mPathId = id; }
        uint32 GetPathId() const { return mPathId; }
        WorldObject* GetBaseObject()
        {
            WorldObject* obj = NULL;
            if (me)
                obj = me;
            else if (go)
                obj = go;
            return obj;
        }

        bool IsUnit(WorldObject* obj)
        {
            return obj && (obj->GetTypeId() == TYPEID_UNIT || obj->GetTypeId() == TYPEID_PLAYER);
        }

        bool IsPlayer(WorldObject* obj)
        {
            return obj && obj->GetTypeId() == TYPEID_PLAYER;
        }

        bool IsCreature(WorldObject* obj)
        {
            return obj && obj->GetTypeId() == TYPEID_UNIT;
        }

        bool IsGameObject(WorldObject* obj)
        {
            return obj && obj->GetTypeId() == TYPEID_GAMEOBJECT;
        }

        void OnUpdate(const uint32 diff);
        void OnMoveInLineOfSight(Unit* who);

        Unit* DoSelectLowestHpFriendly(float range, uint32 MinHPDiff);
        void DoFindFriendlyCC(std::list<Creature*>& _list, float range);
        void DoFindFriendlyMissingBuff(std::list<Creature*>& list, float range, uint32 spellid);
        Unit* DoFindClosestFriendlyInRange(float range, bool playerOnly);

        void StoreTargetList(ObjectList* targets, uint32 id)
        {
            if (!targets)
                return;

            if (mTargetStorage->find(id) != mTargetStorage->end())
            {
                // check if already stored
                if ((*mTargetStorage)[id]->Equals(targets))
                    return;

                delete (*mTargetStorage)[id];
            }

            (*mTargetStorage)[id] = new ObjectGuidList(targets, GetBaseObject());
        }

        bool IsSmart(Creature* c = NULL)
        {
            bool smart = true;
            if (c && c->GetAIName() != "SmartAI")
                smart = false;

            if (!me || me->GetAIName() != "SmartAI")
                smart = false;

            if (!smart)
                TC_LOG_ERROR("sql.sql", "SmartScript: Action target Creature (GUID: %u Entry: %u) is not using SmartAI, action called by Creature (GUID: %u Entry: %u) skipped to prevent crash.", c ? c->GetDBTableGUIDLow() : 0, c ? c->GetEntry() : 0, me ? me->GetDBTableGUIDLow() : 0, me ? me->GetEntry() : 0);

            return smart;
        }

        bool IsSmartGO(GameObject* g = NULL)
        {
            bool smart = true;
            if (g && g->GetAIName() != "SmartGameObjectAI")
                smart = false;

            if (!go || go->GetAIName() != "SmartGameObjectAI")
                smart = false;
            if (!smart)
                TC_LOG_ERROR("sql.sql", "SmartScript: Action target GameObject (GUID: %u Entry: %u) is not using SmartGameObjectAI, action called by GameObject (GUID: %u Entry: %u) skipped to prevent crash.", g ? g->GetDBTableGUIDLow() : 0, g ? g->GetEntry() : 0, go ? go->GetDBTableGUIDLow() : 0, go ? go->GetEntry() : 0);

            return smart;
        }

        ObjectList* GetTargetList(uint32 id)
        {
            ObjectListMap::iterator itr = mTargetStorage->find(id);
            if (itr != mTargetStorage->end())
                return (*itr).second->GetObjectList();
            return NULL;
        }

        void StoreCounter(uint32 id, uint32 value, uint32 reset)
        {
            CounterMap::const_iterator itr = mCounterList.find(id);
            if (itr != mCounterList.end())
            {
                if (reset == 0)
                    value += GetCounterValue(id);
                mCounterList.erase(id);
            }

            mCounterList.insert(std::make_pair(id, value));
            ProcessEventsFor(SMART_EVENT_COUNTER_SET);
        }

        uint32 GetCounterId(uint32 id)
        {
            CounterMap::iterator itr = mCounterList.find(id);
            if (itr != mCounterList.end())
                return itr->first;
            return 0;
        }

        uint32 GetCounterValue(uint32 id)
        {
            CounterMap::iterator itr = mCounterList.find(id);
            if (itr != mCounterList.end())
                return itr->second;
            return 0;
        }

        GameObject* FindGameObjectNear(WorldObject* searchObject, uint32 guid) const
        {
            GameObject* gameObject = NULL;

            CellCoord p(Trinity::ComputeCellCoord(searchObject->GetPositionX(), searchObject->GetPositionY()));
            Cell cell(p);

            Trinity::GameObjectWithDbGUIDCheck goCheck(*searchObject, guid);
            Trinity::GameObjectSearcher<Trinity::GameObjectWithDbGUIDCheck> checker(searchObject, gameObject, goCheck);

            TypeContainerVisitor<Trinity::GameObjectSearcher<Trinity::GameObjectWithDbGUIDCheck>, GridTypeMapContainer > objectChecker(checker);
            cell.Visit(p, objectChecker, *searchObject->GetMap(), *searchObject, searchObject->GetGridActivationRange());

            return gameObject;
        }

        Creature* FindCreatureNear(WorldObject* searchObject, uint32 guid) const
        {
            Creature* creature = NULL;
            CellCoord p(Trinity::ComputeCellCoord(searchObject->GetPositionX(), searchObject->GetPositionY()));
            Cell cell(p);

            Trinity::CreatureWithDbGUIDCheck target_check(searchObject, guid);
            Trinity::CreatureSearcher<Trinity::CreatureWithDbGUIDCheck> checker(searchObject, creature, target_check);

            TypeContainerVisitor<Trinity::CreatureSearcher <Trinity::CreatureWithDbGUIDCheck>, GridTypeMapContainer > unit_checker(checker);
            cell.Visit(p, unit_checker, *searchObject->GetMap(), *searchObject, searchObject->GetGridActivationRange());

            return creature;
        }

        ObjectListMap* mTargetStorage;

        void OnReset();
        void ResetBaseObject()
        {
            if (meOrigGUID)
            {
                if (Creature* m = HashMapHolder<Creature>::Find(meOrigGUID))
                {
                    me = m;
                    go = NULL;
                }
            }
            if (goOrigGUID)
            {
                if (GameObject* o = HashMapHolder<GameObject>::Find(goOrigGUID))
                {
                    me = NULL;
                    go = o;
                }
            }
            goOrigGUID.Clear();
            meOrigGUID.Clear();
        }

        //TIMED_ACTIONLIST (script type 9 aka script9)
        void SetScript9(SmartScriptHolder& e, uint32 entry);
        Unit* GetLastInvoker();
        ObjectGuid mLastInvoker;
        typedef std::unordered_map<uint32, uint32> CounterMap;
        CounterMap mCounterList;

    private:
        void IncPhase(int32 p = 1)
        {
            if (p >= 0)
                mEventPhase += (uint32)p;
            else
                DecPhase(abs(p));
        }

        void DecPhase(int32 p = 1) 
        { 
            if(mEventPhase > (uint32)p)
                mEventPhase -= (uint32)p; 
            else
                mEventPhase = 0;
        }

        bool IsInPhase(uint32 p) const { return ((1 << (mEventPhase - 1)) & p) != 0; }
        void SetPhase(uint32 p = 0) { mEventPhase = p; }

        SmartAIEventList mEvents;
        SmartAIEventList mInstallEvents;
        SmartAIEventList mTimedActionList;
        bool isProcessingTimedActionList;
        Creature* me;
        ObjectGuid meOrigGUID;
        GameObject* go;
        ObjectGuid goOrigGUID;
        AreaTriggerEntry const* trigger;
        SmartScriptType mScriptType;
        uint32 mEventPhase;

        uint32 mPathId;
        SmartAIEventList mStoredEvents;
        std::list<uint32>mRemIDs;

        uint32 mTextTimer;
        uint32 mLastTextID;
        uint32 mTalkerEntry;
        bool mUseTextTimer;

        SMARTAI_TEMPLATE mTemplate;
        void InstallEvents();

        void RemoveStoredEvent(uint32 id)
        {
            if (!mStoredEvents.empty())
            {
                for (SmartAIEventList::iterator i = mStoredEvents.begin(); i != mStoredEvents.end(); ++i)
                {
                    if (i->event_id == id)
                    {
                        mStoredEvents.erase(i);
                        return;
                    }
                }
            }
        }
};

#endif

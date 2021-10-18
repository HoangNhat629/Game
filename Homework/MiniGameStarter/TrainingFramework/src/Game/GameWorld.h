#pragma once

#include "Entity/Entity.h"
#include "Entity/PlayerEntity.h"
#include "Entity/KeyEntity.h"

#include "GridSlot.h"

#include "Camera.h"
#include "Sprite2D.h"
#include "AtlasSprite2D.h"

#include "SpriteUtils/SpriteLoader.h"

#include "StateMachine/StateMachine.h"

#include "ApplicationManagers/SoundManager.h"

#include <memory>
#include <vector>
#include <unordered_map>

namespace Agvt
{
    class GameWorld
    {
    public:
        enum class GravityDirection
        {
            Down = 0,
            Right = 1,
            Up = 2,
            Left = 3
        };

        enum class InputDirection
        {
            Left = 0,
            Right = 1
        };

    public:
        GameWorld();

        void Init(int level);

        void Update(float deltaTime);
        void Draw();

        bool IsGameOver() const;
        bool IsVictory() const;

        bool IsInvalid() const;

    private:
        void SetGravity(GravityDirection gravityDirection);
        void SetWorldRotation(float angle);

        friend class InvalidState;
        friend class PlayState;
        friend class GravitySelectState;
        friend class UpdateState;
        friend class EndState;

    private:
        bool LoadMap(int id);
        bool AddEntity(const SpriteLoader& spriteLoader, EntityType entityType, Vector2Int position);
        bool CreateTiles(const SpriteLoader& spriteLoader, const std::vector<uint8_t>& tiles);

        bool TryMoveEntityUnderGravity(Entity* entity, Vector2Int newPosition);
        bool TryMovePlayerEntityUnderGravity(Entity* entity, Vector2Int newPosition, GridSlot& currentGridSlot, GridSlot& targetGridSlot);
        bool TryMoveBoxEntityUnderGravity(Entity* entity, Vector2Int newPosition, GridSlot& currentGridSlot, GridSlot& targetGridSlot);
        bool TryMoveRockEntityUnderGravity(Entity* entity, Vector2Int newPosition, GridSlot& currentGridSlot, GridSlot& targetGridSlot);

        bool TryMovePlayerUnderInput(InputDirection inputDirection);
        bool TryMoveBoxEntityUnderInput(Entity* entity, Vector2Int direction);

        void OnPlayerPickupKey();
        void OnEntityGoOutOfBounds(Entity* entity, Vector2Int newPosition, GridSlot& currentGridSlot);

        void OnPlayerDie();
        void OnPlayerExit();

        void MarkWorldAsChanged();
        bool ApplyGravity();

        void MarkEntityAsRemoved(Entity* entity);
        void ProcessEntityRemovePendingList();

        size_t GridPos2Index(Vector2Int position) const;
        static constexpr Vector2Int GravityDirection2Vector(GravityDirection gravityDirection);

    private:
        std::shared_ptr<Camera> m_worldCamera;

        std::vector<std::unique_ptr<Sprite2D>> m_tiles;
        std::vector<std::unique_ptr<Entity>> m_entities;
        std::vector<std::unique_ptr<Entity>> m_keys;

        AtlasSprite2D* m_doorTile;

        PlayerEntity* m_player;
        uint8_t m_keysToCollectCount;
        bool m_gameOver;
        bool m_playerExited;

        GravityDirection m_currentGravityDirection;


        uint8_t m_mapWidth;
        uint8_t m_mapHeight;
        std::vector<GridSlot> m_mapLookup;
        std::unordered_map<Vector2Int, Entity*, Vector2IntHasher> m_keyLookup;

        std::vector<Entity*> m_entityUpdateQueue;
        bool m_entitiesListChanged;
        bool m_keysListChanged;

        StateMachine m_stateMachine;

        SoundManager* m_soundManager;
    };
}
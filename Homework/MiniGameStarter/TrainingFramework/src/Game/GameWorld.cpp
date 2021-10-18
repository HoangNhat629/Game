#include "GameWorld.h"

#include "GameWorldConfig.h"
#include "Map/GameMap.h"

#include "Entity/BoxEntity.h"
#include "Entity/RockEntity.h"

#include "ResourceManagers.h"

#include "Application.h"

#include <array>
#include <algorithm>
#include <assert.h>

constexpr float c_updateDelayTime = 0.2f;
constexpr float c_updateDelayAfterPlayerMovement = 0.1f;

namespace Agvt
{
    using namespace GameWorldConfig;

    GameWorld::GameWorld() : m_worldCamera{}, m_tiles{}, m_entities{}, m_keys{}, m_doorTile{}, m_player{},
        m_keysToCollectCount{}, m_gameOver(false), m_playerExited(false),
        m_currentGravityDirection{},
        m_mapWidth{}, m_mapHeight{}, m_mapLookup{}, m_keyLookup{}, m_entityUpdateQueue{}, m_entitiesListChanged(false), m_keysListChanged(false),
        m_soundManager{}
    {
    }

    void GameWorld::Init(int level)
    {
        m_worldCamera = std::make_shared<Camera>(0, 0.0f, c_mapWidth, c_mapHeight, 0.0f, -1.0f, 1.0f, 0.0f);
        m_stateMachine.Init(this, Application::GetInstance()->GetInputManager());

        m_soundManager = Application::GetInstance()->GetSoundManager();

        if (LoadMap(level))
        {
            m_stateMachine.ChangeState(StateType::Updating);
        }
        else
        {
            LOGI("Invalid map construction\n");
        }
    }

    void GameWorld::Update(float deltaTime)
    {
        if (IsInvalid())
        {
            return;
        }

        m_stateMachine.Update(deltaTime);

        if (!m_gameOver)
        {
            m_player->Update(deltaTime);
        }
    }

    void GameWorld::Draw()
    {
        if (IsInvalid())
        {
            return;
        }

        for (auto& tile : m_tiles)
        {
            tile->Draw();
        }

        for (auto& entity : m_entities)
        {
            if (entity->IsValid())
            {
                entity->Draw();
            }
        }

        for (auto& key : m_keys)
        {
            if (key->IsValid())
            {
                key->Draw();
            }
        }
    }

    bool GameWorld::IsGameOver() const
    {
        return m_gameOver;
    }

    bool GameWorld::IsVictory() const
    {
        return m_playerExited;
    }

    bool GameWorld::IsInvalid() const
    {
        return m_stateMachine.GetCurrentStateType() == StateType::Invalid;
    }

    void GameWorld::SetGravity(GravityDirection gravityDirection)
    {
        if (gravityDirection == GravityDirection::Down)
        {
            return;
        }

        size_t gravityIndex = (size_t)m_currentGravityDirection + (size_t)gravityDirection;
        if (gravityIndex >= 4)
        {
            gravityIndex -= 4;
        }

        m_currentGravityDirection = (GravityDirection)gravityIndex;
    }

    void GameWorld::SetWorldRotation(float angle)
    {
        // Rotate camera
        m_worldCamera->BeginUpdate()
            .SetRotationZ(-angle)
            .ApplyChanges();

        // Rotate entities
        for (auto& entity : m_entities)
        {
            entity->SetRotation(Vector3(0, 0, angle));
        }
    }

    bool GameWorld::LoadMap(int id)
    {
        GameMap gameMap;
        if (!GameMap::LoadFromFile(id, &gameMap))
        {
            LOGI("Cannot load map %d\n", id);
            return false;
        }

        m_mapWidth = gameMap.Width();
        m_mapHeight = gameMap.Height();

        m_mapLookup = std::vector<GridSlot>(m_mapWidth * m_mapHeight);

        SpriteLoader spriteLoader;

        // Create tiles
        if (!CreateTiles(spriteLoader, gameMap.Tiles()))
        {
            return false;
        }

        // Create entities
        const std::vector<GameMap::EntityEntry>& entityEntries = gameMap.Entities();

        for (const auto& entityEntry : entityEntries)
        {
            if (!AddEntity(spriteLoader, (EntityType)entityEntry.entityType, Vector2Int(entityEntry.x, entityEntry.y)))
            {
                return false;
            }
        }

        return true;
    }

    bool GameWorld::AddEntity(const SpriteLoader& spriteLoader, EntityType entityType, Vector2Int position)
    {
        Entity* movableEntity = nullptr;

        std::unique_ptr<Sprite2D> sprite = spriteLoader.LoadEntitySprite(entityType, position);

        switch (entityType)
        {
        case EntityType::Player: {
            std::unique_ptr<PlayerEntity> playerEntity = std::make_unique<PlayerEntity>(std::move(sprite));
            movableEntity = playerEntity.get();
            m_player = playerEntity.get();

            playerEntity->SetCamera(m_worldCamera);
            playerEntity->SetPosition(position);

            m_entities.push_back(std::move(playerEntity));
            break;
        }
        case EntityType::Rock: {
            std::unique_ptr<RockEntity> rockEntity = std::make_unique<RockEntity>(std::move(sprite));
            movableEntity = rockEntity.get();

            rockEntity->SetCamera(m_worldCamera);
            rockEntity->SetPosition(position);

            m_entities.push_back(std::move(rockEntity));
            break;
        }
        case EntityType::Box: {
            std::unique_ptr<BoxEntity> boxEntity = std::make_unique<BoxEntity>(std::move(sprite));
            movableEntity = boxEntity.get();

            boxEntity->SetPosition(position);
            boxEntity->SetCamera(m_worldCamera);

            m_entities.push_back(std::move(boxEntity));
            break;
        }
        case EntityType::Key: {
            std::unique_ptr<KeyEntity> keyEntity = std::make_unique<KeyEntity>(std::move(sprite));

            keyEntity->SetPosition(position);
            keyEntity->SetCamera(m_worldCamera);

            bool insertResult = m_keyLookup.insert({ position, keyEntity.get() }).second;
            if (!insertResult)
            {
                return false;
            }

            m_keys.push_back(std::move(keyEntity));

            m_keysToCollectCount++;
            break;
        }
        default:
            break;
        }

        if (entityType != EntityType::Key)
        {
            GridSlot& gridSlot = m_mapLookup[GridPos2Index(position)];
            GridSlot::ContentType slotContentType = gridSlot.Type();

            if (slotContentType == GridSlot::ContentType::Empty)
            {
                gridSlot = GridSlot(movableEntity);
            }
            else
            {
                return false;
            }
        }

        return true;
    }

    bool GameWorld::CreateTiles(const SpriteLoader& spriteLoader, const std::vector<uint8_t>& tiles)
    {
        for (size_t y = 0; y < m_mapHeight; y++)
        {
            for (size_t x = 0; x < m_mapWidth; x++)
            {
                std::pair<uint8_t, uint8_t> tileData = GameMap::UnpackTileData(tiles[GridPos2Index(Vector2Int(x, y))]);
                TileType tileType = (TileType)tileData.first;
                uint8_t tileDetails = tileData.second;

                if (tileType != TileType::Empty)
                {
                    std::unique_ptr<Sprite2D> tileSprite;
                    if (tileType == TileType::Exit)
                    {
                        tileSprite = spriteLoader.LoadTileSprite(tileType, tileDetails, Vector2Int(x, y));
                    }
                    else
                    {
                        tileSprite = spriteLoader.LoadTileSprite(tileType, tileDetails, Vector2Int(x, y));
                    }
                    tileSprite->SetCamera(m_worldCamera);

                    GridSlot& gridSlot = m_mapLookup[GridPos2Index(Vector2Int(x, y))];
                    gridSlot = GridSlot(tileType);

                    if (tileType == TileType::Exit)
                    {
                        m_doorTile = static_cast<AtlasSprite2D*>(tileSprite.get());
                    }

                    m_tiles.push_back(std::move(tileSprite));
                }
            }
        }

        return m_doorTile != nullptr;
    }


    bool GameWorld::TryMoveEntityUnderGravity(Entity* entity, Vector2Int newPosition)
    {
        GridSlot& currentGridSlot = m_mapLookup[GridPos2Index(entity->GetGridPosition())];

        // Check bounds
        if (newPosition.x < 0 || newPosition.x >= m_mapWidth || newPosition.y < 0 || newPosition.y >= m_mapHeight)
        {
            OnEntityGoOutOfBounds(entity, newPosition, currentGridSlot);
            return true;
        }

        GridSlot& targetGridSlot = m_mapLookup[GridPos2Index(newPosition)];

        switch (entity->GetType())
        {
        case EntityType::Player:
            return TryMovePlayerEntityUnderGravity(entity, newPosition, currentGridSlot, targetGridSlot);
            break;
        case EntityType::Box:
            return TryMoveBoxEntityUnderGravity(entity, newPosition, currentGridSlot, targetGridSlot);
            break;
        case EntityType::Rock:
            return TryMoveRockEntityUnderGravity(entity, newPosition, currentGridSlot, targetGridSlot);
            break;
        default:
            assert(false && "This entity is not supposed to move");
            break;
        }

        return false;
    }

    bool GameWorld::TryMovePlayerEntityUnderGravity(Entity* entity, Vector2Int newPosition, GridSlot& currentGridSlot, GridSlot& targetGridSlot)
    {
        if (targetGridSlot.Type() == GridSlot::ContentType::Empty)
        {
            // Check for key
            auto keyIterator = m_keyLookup.find(newPosition);
            if (keyIterator != m_keyLookup.end())
            {
                // Move to a key
                // Remove key
                MarkEntityAsRemoved(keyIterator->second);

                // Update lookup
                m_keyLookup.erase(keyIterator);

                OnPlayerPickupKey();
            }

            // Update grid slots
            currentGridSlot = GridSlot();
            targetGridSlot = GridSlot(entity);

            // Update position
            entity->SetPosition(newPosition);

            // Update animation
            m_player->SetAnimationState(PlayerEntity::AnimationState::Falling);

            return true;
        }

        // Blocked by tile or another entity
        // Update animation
        m_player->SetAnimationState(PlayerEntity::AnimationState::Idle);

        return false;
    }

    bool GameWorld::TryMoveBoxEntityUnderGravity(Entity* entity, Vector2Int newPosition, GridSlot& currentGridSlot, GridSlot& targetGridSlot)
    {
        if (targetGridSlot.Type() == GridSlot::ContentType::Empty)
        {
            // Update grid slot
            currentGridSlot = GridSlot();
            targetGridSlot = GridSlot(entity);

            // Update position
            entity->SetPosition(newPosition);

            return true;
        }

        // Blocked by tile or another entity
        return false;
    }

    bool GameWorld::TryMoveRockEntityUnderGravity(Entity* entity, Vector2Int newPosition, GridSlot& currentGridSlot, GridSlot& targetGridSlot)
    {
        if (targetGridSlot.Type() == GridSlot::ContentType::Tile)
        {
            return false;
        }

        if (targetGridSlot.Type() == GridSlot::ContentType::Empty)
        {
            // Update grid slot
            currentGridSlot = GridSlot();
            targetGridSlot = GridSlot(entity);

            // Update position
            entity->SetPosition(newPosition);

            return true;
        }

        EntityType slotEntityType = targetGridSlot.AsEntity()->GetType();
        if (slotEntityType == EntityType::Box)
        {
            // Move to a box
            // Destroy box
            MarkEntityAsRemoved(targetGridSlot.AsEntity());

            // Update grid slots
            currentGridSlot = GridSlot();
            targetGridSlot = GridSlot(entity);

            // Update position
            entity->SetPosition(newPosition);

            return true;
        }

        if (slotEntityType == EntityType::Player)
        {
            // Move to player
            // Destroy player
            MarkEntityAsRemoved(m_player);
            m_player = nullptr;

            OnPlayerDie();

            // Update grid slots
            currentGridSlot = GridSlot();
            targetGridSlot = GridSlot(entity);

            // Update position
            entity->SetPosition(newPosition);

            return true;
        }

        // Blocked
        return false;
    }

    bool GameWorld::TryMovePlayerUnderInput(InputDirection inputDirection)
    {
        // Update animation
        m_player->SetFacingDirection(inputDirection == InputDirection::Right);

        constexpr std::array<Vector2Int, 4> gravityIndexToLeftVector
        {
            Vector2Int::Left(),
            Vector2Int::Down(),
            Vector2Int::Right(),
            Vector2Int::Up()
        };

        Vector2Int leftVector = gravityIndexToLeftVector[(size_t)m_currentGravityDirection];
        Vector2Int direction = inputDirection == InputDirection::Left ? leftVector : -leftVector;

        Entity* entity = m_player;
        Vector2Int newPosition = entity->GetGridPosition() + direction;

        // Check bounds
        if (newPosition.x < 0 || newPosition.x >= m_mapWidth || newPosition.y < 0 || newPosition.y >= m_mapHeight)
        {
            return false;
        }

        GridSlot& currentGridSlot = m_mapLookup[GridPos2Index(entity->GetGridPosition())];
        GridSlot& targetGridSlot = m_mapLookup[GridPos2Index(newPosition)];

        if (targetGridSlot.Type() == GridSlot::ContentType::Tile)
        {
            // Check moving to the door
            if (targetGridSlot.AsTile() == TileType::Exit && m_keysToCollectCount == 0 && m_currentGravityDirection == GravityDirection::Down)
            {
                currentGridSlot = GridSlot();
                OnPlayerExit();

                // Update position
                entity->SetPosition(newPosition);

                return true;
            }

            return false;
        }

        while (true)
        {
            if (targetGridSlot.Type() == GridSlot::ContentType::Empty)
            {
                // Check for key
                auto keyIterator = m_keyLookup.find(newPosition);
                if (keyIterator != m_keyLookup.end())
                {
                    // Move to a key
                    // Remove key
                    MarkEntityAsRemoved(keyIterator->second);

                    // Update lookup
                    m_keyLookup.erase(keyIterator);

                    OnPlayerPickupKey();
                }

                // Update grid slots
                currentGridSlot = GridSlot();
                targetGridSlot = GridSlot(entity);

                // Update position
                entity->SetPosition(newPosition);

                return true;
            }

            EntityType targetSlotEntityType = targetGridSlot.AsEntity()->GetType();

            // Move to a rock
            if (targetSlotEntityType == EntityType::Rock)
            {
                return false;
            }

            // Move to a box
            if (targetSlotEntityType == EntityType::Box)
            {
                // Try to move the blocking box out of the way
                if (!TryMoveBoxEntityUnderInput(targetGridSlot.AsEntity(), direction))
                {
                    return false;
                }
            }

            // Try to move again
        }

        // Blocked
        return false;
    }

    bool GameWorld::TryMoveBoxEntityUnderInput(Entity* entity, Vector2Int direction)
    {
        Vector2Int newPosition = entity->GetGridPosition() + direction;
        GridSlot& currentGridSlot = m_mapLookup[GridPos2Index(entity->GetGridPosition())];

        // Check bounds
        if (newPosition.x < 0 || newPosition.x >= m_mapWidth || newPosition.y < 0 || newPosition.y >= m_mapHeight)
        {
            OnEntityGoOutOfBounds(entity, newPosition, currentGridSlot);
            return true;
        }

        GridSlot& targetGridSlot = m_mapLookup[GridPos2Index(newPosition)];

        if (targetGridSlot.Type() == GridSlot::ContentType::Tile)
        {
            return false;
        }

        while (true)
        {
            if (targetGridSlot.Type() == GridSlot::ContentType::Empty)
            {
                // Update grid slots
                currentGridSlot = GridSlot();
                targetGridSlot = GridSlot(entity);

                // Update position
                entity->SetPosition(newPosition);

                return true;
            }

            EntityType targetSlotEntityType = targetGridSlot.AsEntity()->GetType();

            // Move to a rock
            if (targetSlotEntityType == EntityType::Rock)
            {
                return false;
            }

            // Moving to a slot that contains another box
            if (targetSlotEntityType == EntityType::Box)
            {
                // Try to move the blocking box out of the way
                if (!TryMoveBoxEntityUnderInput(targetGridSlot.AsEntity(), direction))
                {
                    return false;
                }
            }

            // Try to move this box again
        }

        // Blocked
        return false;
    }

    void GameWorld::OnPlayerPickupKey()
    {
        m_keysToCollectCount--;
        if (m_keysToCollectCount == 0)
        {
            m_doorTile->SetAtlasCoord(Vector2Int(1, 0));
        }

        m_soundManager->PlayKeyPickupSound();
    }

    void GameWorld::OnEntityGoOutOfBounds(Entity* entity, Vector2Int newPosition, GridSlot& currentGridSlot)
    {
        MarkEntityAsRemoved(entity);

        if (entity->GetType() == EntityType::Player)
        {
            m_player = nullptr;

            OnPlayerDie();
        }

        // Update grid slots
        currentGridSlot = GridSlot();
    }

    void GameWorld::OnPlayerDie()
    {
        m_gameOver = true;
        m_soundManager->PlayFailSound();
    }

    void GameWorld::OnPlayerExit()
    {
        m_playerExited = true;
        m_soundManager->PlayCompleteSound();
    }

    void GameWorld::MarkWorldAsChanged()
    {
        // Queue up entities to be updated
        // List of entities will be sorted according to vertical component of gravity vector
        m_entityUpdateQueue.clear();
        for (const auto& entity : m_entities)
        {
            m_entityUpdateQueue.push_back(entity.get());
        }

        switch (m_currentGravityDirection)
        {
        case GravityDirection::Down:
            std::sort(m_entityUpdateQueue.begin(), m_entityUpdateQueue.end(), [](const Entity* left, const Entity* right)
            {
                return left->GetGridPosition().y < right->GetGridPosition().y;
            });
            break;
        case GravityDirection::Left:
            std::sort(m_entityUpdateQueue.begin(), m_entityUpdateQueue.end(), [](const Entity* left, const Entity* right)
            {
                return left->GetGridPosition().x < right->GetGridPosition().x;
            });
            break;
        case GravityDirection::Up:
            std::sort(m_entityUpdateQueue.begin(), m_entityUpdateQueue.end(), [](const Entity* left, const Entity* right)
            {
                return left->GetGridPosition().y > right->GetGridPosition().y;
            });
            break;
        case GravityDirection::Right:
            std::sort(m_entityUpdateQueue.begin(), m_entityUpdateQueue.end(), [](const Entity* left, const Entity* right)
            {
                return left->GetGridPosition().x > right->GetGridPosition().x;
            });
            break;
        }
    }

    bool GameWorld::ApplyGravity()
    {
        Vector2Int gravityVector = GravityDirection2Vector(m_currentGravityDirection);

        bool needToUpdate = false;
        for (auto entity : m_entityUpdateQueue)
        {
            if (!entity->IsValid())
            {
                continue;
            }

            Vector2Int newPosition = entity->GetGridPosition() + gravityVector;
            if (TryMoveEntityUnderGravity(entity, newPosition))
            {
                needToUpdate = true;
            }
        }

        return needToUpdate;
    }


    void GameWorld::MarkEntityAsRemoved(Entity* entity)
    {
        entity->Invalidate();

        if (entity->GetType() == EntityType::Key)
        {
            m_keysListChanged = true;
        }
        else
        {
            m_entitiesListChanged = true;
        }
    }

    void GameWorld::ProcessEntityRemovePendingList()
    {
        if (m_entitiesListChanged)
        {
            m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(), [](const std::unique_ptr<Entity>& entity)
            {
                return !entity->IsValid();
            }), m_entities.end());

            m_entitiesListChanged = false;
        }

        if (m_keysListChanged)
        {
            m_keys.erase(std::remove_if(m_keys.begin(), m_keys.end(), [](const std::unique_ptr<Entity>& keyEntity)
            {
                return !keyEntity->IsValid();
            }), m_keys.end());

            m_keysListChanged = false;
        }
    }

    size_t GameWorld::GridPos2Index(Vector2Int position) const
    {
        return position.x + (size_t)m_mapWidth * position.y;
    }

    constexpr Vector2Int GameWorld::GravityDirection2Vector(GravityDirection gravityDirection)
    {
        constexpr std::array<Vector2Int, 4> directionToVector
        {
            Vector2Int::Down(),
            Vector2Int::Right(),
            Vector2Int::Up(),
            Vector2Int::Left()
        };

        return directionToVector[(size_t)gravityDirection];
    }

}
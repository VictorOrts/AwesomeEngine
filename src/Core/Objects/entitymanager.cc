#include "Core/Objects/entitiymanager.h"
//#include "Core/Objects/entity.h"
using namespace Awesome;

EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{
}

std::shared_ptr<Entity> EntityManager::Create() {
	std::shared_ptr<Entity> tmpEntity = std::make_shared<Entity>();
	entityList_.push_back(tmpEntity);
		return tmpEntity;
}
void EntityManager::registerTag(const string tagName, std::shared_ptr<Entity> a) {
	tagByEntity.insert(std::pair<string, std::shared_ptr<Entity>>(tagName, a));
}
void EntityManager::unregisterTag(const string tagName) {
	tagByEntity.erase(tagName);
}
bool EntityManager::isRegistered(const string tagName) {
	return (tagByEntity[tagName] != NULL);
}
std::shared_ptr<Entity> EntityManager::getEntity(const string tagName) {
	return tagByEntity[tagName];
}

std::vector<std::shared_ptr<Entity>> EntityManager::getList()
{
	return entityList_;
}

void EntityManager::removeEntity(std::shared_ptr<Entity> ent) {
	std::map<string, std::shared_ptr<Entity>>::iterator it;
	for (it = tagByEntity.begin(); it != tagByEntity.end(); ++it) {
		if (it->second == ent) {
			tagByEntity.erase(it->first);
			return;
		}
	}
}
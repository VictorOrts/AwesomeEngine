#ifndef _H_ENTITY_MANAGER_
#define _H_ENTITY_MANAGER_


#include "Core/types.h"
#include "entity.h"
#include <memory>

namespace Awesome {
	class EntityManager
	{
		//class Entity;
	public:
		EntityManager();
		~EntityManager();
		std::shared_ptr<Entity> Create();
		void registerTag(const string, std::shared_ptr<Entity>);
		void unregisterTag(const string);
		bool isRegistered(const string);
		void removeEntity(std::shared_ptr<Entity>);
		std::shared_ptr<Entity> getEntity(const string);
		std::vector<std::shared_ptr<Entity>> getList();
	private:
		std::map<string, std::shared_ptr<Entity>> tagByEntity;
		std::vector<std::shared_ptr<Entity>> entityList_;
	};


}
#endif // !_H_ENTITY_MANAGER_

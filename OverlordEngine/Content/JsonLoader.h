#pragma once
#include <Misc/json.hpp>

using json = nlohmann::json;

class JsonLoader : public ContentLoader<json>
{
public:
	JsonLoader(const JsonLoader& other) = delete;
	JsonLoader(JsonLoader&& other) noexcept = delete;
	JsonLoader& operator=(const JsonLoader& other) = delete;
	JsonLoader& operator=(JsonLoader&& other) noexcept = delete;
	JsonLoader() = default;
	~JsonLoader() override = default;

protected:
	json* LoadContent(const ContentLoadInfo& loadInfo) override;
	void Destroy(json* objToDestroy) override;
};


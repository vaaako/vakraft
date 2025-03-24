#include "vakraft/blocks/block.hpp"
#include "vakraft/blocks/models/block_factory.hpp"

Block::Block() noexcept : id(0), name("air"), is_transparent(false), is_cube(false) {}

Block::Block(const Block::Config& config) noexcept
	: id(config.id), name(config.name), model_type(config.model_type),
	  is_transparent(config.is_transparent), is_cube(config.is_cube),
	  texuv(BlockFactory::get_texuv(config.model_type)) {}

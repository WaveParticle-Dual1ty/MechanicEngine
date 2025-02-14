#pragma once
#include "MechanicEngine/Include/Core/Logging.h"

#define MEPIC_LOG_TRACE(fmt, ...) ME_LOG_TRACE("MEPic", fmt, ##__VA_ARGS__)
#define MEPIC_LOG_DEBUG(fmt, ...) ME_LOG_DEBUG("MEPic", fmt, ##__VA_ARGS__)
#define MEPIC_LOG_INFO(fmt, ...) ME_LOG_INFO("MEPic", fmt, ##__VA_ARGS__)
#define MEPIC_LOG_WARN(fmt, ...) ME_LOG_WARN("MEPic", fmt, ##__VA_ARGS__)
#define MEPIC_LOG_ERROR(fmt, ...) ME_LOG_ERROR("MEPic", fmt, ##__VA_ARGS__)

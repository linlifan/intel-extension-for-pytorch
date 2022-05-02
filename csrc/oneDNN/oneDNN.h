#pragma once

#include <ATen/ATen.h>
#include <oneDNN/LRUCache.h>
#include <oneDNN/Runtime.h>
#include <oneDNN/Utils.h>

#include "BatchNorm.h"
#include "Binary.h"
#include "Concat.h"
#include "Conv.h"
#include "Deconv.h"
#include "Eltwise.h"
#include "GRU.h"
#include "LSTM.h"
#include "LayerNorm.h"
#include "Matmul.h"
#include "Pooling.h"
#include "Reduce.h"
#include "Reorder.h"
#include "SoftMax.h"
#include "Sum.h"
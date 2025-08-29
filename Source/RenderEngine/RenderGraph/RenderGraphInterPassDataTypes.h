#pragma once
#include "MathDefines.h"
namespace WaxGourd {
struct FrameData {
float4x4 camera_prev_view;
float4x4 camera_view;
float4x4 camera_proj;
float4x4 camera_viewproj;
float	 camera_fov;
float	 camera_aspect_ratio;
float	 camera_near;
float	 camera_far;
float	 camera_position[4];

// Temporal stuff
float	 camera_jitter_x;
float	 camera_jitter_y;
float	 delta_time;

// 
uint64	 frame_constant_buffer_address;
};

struct DoFData {
    float dof_focus_distance;
    float dof_focus_radius;
};
}
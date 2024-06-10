#include "../headers/Bloom.h"

Bloom::Bloom(int height, int width)
{
    glGenFramebuffers(1, &fbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

    glGenTextures(1, &downscaled_);
    glBindTexture(GL_TEXTURE_2D, downscaled_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0.5f * float(width), 0.5f * float(height), 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, downscaled_, 0);

    glGenTextures(1, &horizontal_);
    glBindTexture(GL_TEXTURE_2D, horizontal_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, float(width),float(height), 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, horizontal_, 0);

    glGenTextures(1, &vertical_);
    glBindTexture(GL_TEXTURE_2D, vertical_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,  float(width),  float(height), 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, vertical_, 0);

    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        exit(1337420);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Bloom::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

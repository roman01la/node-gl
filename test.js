const gl = require("bindings")("node-gl.node");
const ReactReconciler = require("react-reconciler");
const React = require("react");

gl.layout();

const h = React.createElement;

class GLView {
  constructor({ style = {} }) {
    Object.assign(this, { style });
    this.children = [];
  }
  update(props) {
    if (props && props.style) {
      this.style = props.style;
    }
    gl.beginPath();
    let { shouldStroke } = style(this.style);

    if (shouldStroke) {
      gl.stroke();
    }

    gl.fill();
  }
}

const hostConfig = {
  useSyncScheduling: true,
  supportsMutation: true,
  now: Date.now,
  getRootHostContext() {
    return {};
  },
  getChildHostContext() {
    return {};
  },
  shouldSetTextContent() {
    return false;
  },
  prepareForCommit() {},
  resetAfterCommit() {},
  createInstance(type, props) {
    if (type === "View") {
      return new GLView(props);
    }
  },
  finalizeInitialChildren() {
    return false;
  },
  appendChildToContainer(parent, child) {
    parent.child = child;
    child.update();
  },
  appendInitialChild(parent, child) {
    parent.children.push(child);
    child.update();
  },
  prepareUpdate(instance, type, oldProps, newProps) {
    return newProps;
  },
  commitUpdate(instance, update) {
    instance.update(update);
    instance.children.forEach(child => {
      child.update();
    });
  }
};

const GLRenderer = ReactReconciler(hostConfig);

let internalContainerStructure;
const GLReact = {
  render(elements, containerNode = {}, callback) {
    if (!internalContainerStructure) {
      internalContainerStructure = GLRenderer.createContainer(
        containerNode,
        false,
        false
      );
    }
    GLRenderer.updateContainer(
      elements,
      internalContainerStructure,
      null,
      callback
    );
  }
};

class View extends React.Component {
  render() {
    return h("View", this.props);
  }
}

const App = ({ width, height }) => {
  return h(
    React.Fragment,
    {},
    h(View, {
      style: {
        top: 0,
        left: 0,
        width: 240,
        height: height,
        backgroundColor: [111, 67, 94]
      }
    }),
    h(
      View,
      {
        style: {
          top: 0,
          left: 240,
          width: width - 240,
          height: height,
          backgroundColor: [240, 240, 240]
        }
      },
      h(View, {
        style: {
          top: height - 120,
          left: 240,
          width: width - 240,
          height: 120,
          backgroundColor: [255, 255, 255]
        }
      })
    )
  );
};

gl.createWindow(1280, 720, "Node.js", (width, height) => {
  GLReact.render(h(App, { width, height }));
});

function style(styles) {
  if (hasProp(styles, "backgroundColor")) {
    if (styles.backgroundColor.length === 3) {
      gl.fillColor(...styles.backgroundColor, 255);
    } else {
      gl.fillColor(...styles.backgroundColor);
    }
  }
  if (hasProp(styles, "borderColor")) {
    if (styles.backgroundColor.length === 3) {
      gl.strokeColor(...styles.borderColor, 255);
    } else {
      gl.strokeColor(...styles.borderColor);
    }
  }
  if (hasProp(styles, "borderWidth")) {
    gl.strokeWidth(styles.borderWidth);
  }
  if (hasProps(styles, ["top", "left", "width", "height"])) {
    if (hasProp(styles, "borderRadius")) {
      gl.roundedRect(
        styles.left,
        styles.top,
        styles.width,
        styles.height,
        styles.borderRadius
      );
    } else {
      gl.rect(styles.left, styles.top, styles.width, styles.height);
    }
  }

  return {
    shouldStroke:
      hasProp(styles, "borderColor") && hasProp(styles, "borderWidth")
  };
}

function hasProp(obj, prop) {
  return obj.hasOwnProperty(prop);
}

function hasProps(obj, props) {
  return props.every(prop => hasProp(obj, prop));
}
